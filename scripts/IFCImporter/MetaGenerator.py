#!/usr/bin/env python3
# -*- Coding: UTF-8 -*-

# ---------------------------------------------------------------------------
# Open Asset Import Library (ASSIMP)
# ---------------------------------------------------------------------------
#
# Copyright (c) 2006-2010, ASSIMP Development Team
#
# All rights reserved.
#
# Redistribution and use of this software in source and binary forms,
# with or without modification, are permitted provided that the following
# conditions are met:
#
# * Redistributions of source code must retain the above
#   copyright notice, this list of conditions and the
#   following disclaimer.
#
# * Redistributions in binary form must reproduce the above
#   copyright notice, this list of conditions and the
#   following disclaimer in the documentation and/or other
#   materials provided with the distribution.
#
# * Neither the name of the ASSIMP team, nor the names of its
#   contributors may be used to endorse or promote products
#   derived from this software without specific prior
#   written permission of the ASSIMP Development Team.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ---------------------------------------------------------------------------

"""Generate the C++ glue code needed to map EXPRESS to C++"""

import sys, os, re

if sys.version_info < (3, 0):
    print("must use python 3.0 or greater")
    sys.exit(-2)

input_template_h = 'IFCMetaData.h.template'
input_template_cpp = 'IFCMetaData.cpp.template'

output_file_h = os.path.join('..','..','code','IFCMetaData.h')
output_file_cpp = os.path.join('..','..','code','IFCMetaData.cpp')

template_header = "\ttemplate <> aiNode *ExtractMetaData<{type}>(const aiNode *parent, const {type}& in, ConversionData& conv);\n"

template_converter = r"""
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<{type}>(const aiNode *parent, const {type}& in, ConversionData& conv)
{{
	{contents}
}}"""

template_converter_prologue = r"""aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
"""

template_count_field = "\tint numProperties = {numProperties};\n";
template_count_optional = "\tif(in.{name}) numProperties++;\n";
template_alloc_strings = r"""
    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
"""

template_ifcid = '\tdata->Set(0, "IFC_id", in.GetID());\n';

template_set_attribute = '\tdata->Set(index++, "{name}", {value_str});\n'
template_if_exists = '\tif (in.{name})'

template_set_attribute_optional = template_if_exists + ' data->Set(index++, "{name}", {value_str});\n'

template_subnodes = "\tstd::vector<aiNode *> subnodes;\n"

template_subnode_process = r"""const {type} {name} = in.{name}{optional};
    aiNode *nd_{name} = ExtractMetaData(ndnew, {name}, conv);
    subnodes.push_back(nd_{name});"""

template_subnode_epilogue = "\tAppendSubnodes(ndnew, subnodes);\n"

template_converter_epilogue = '\treturn ndnew;'

import ExpressReader

def get_list_bounds(collection_spec):
    start,end = [(int(n) if n!='?' else 0) for n in re.findall(r'(\d+|\?)',collection_spec)]
    return start,end

def get_cpp_type(field,schema):
    isobjref = field.type in schema.entities
    base = field.type
    if isobjref:
        base = 'Lazy< '+(base if base in schema.whitelist else 'NotImplemented')+' >'
    if field.collection:
        start,end = get_list_bounds(field.collection)
        base = 'ListOf< {0}, {1}, {2} >'.format(base,start,end)
    if not isobjref:
        base += '::Out'
    if field.optional:
        base = 'Maybe< '+base+' >'

    return base

def generate_fields(entity,schema):
    fields = []
    for e in entity.members:
        fields.append('\t\t{type} {name};'.format(type=get_cpp_type(e,schema),name=e.name))
    return '\n'.join(fields)

def handle_unset_args(field,entity,schema,argnum):
    n = ''
    # if someone derives from this class, check for derived fields.
    if any(entity.name==e.parent for e in schema.entities.values()):
        n += template_allow_derived.format(type=entity.name,argcnt=len(entity.members),argnum=argnum)

    if not field.optional:
        return n+''
    return n+template_allow_optional.format()

def get_single_conversion(field,schema,argnum=0,classname='?'):
    typen = field.type
    name = field.name
    if field.collection:
        typen = 'LIST'
    return template_convert_single.format(type=typen,name=name,argnum=argnum,classname=classname,full_type=field.fullspec)

def count_args_up(entity,schema):
    return len(entity.members) + (count_args_up(schema.entities[entity.parent],schema) if entity.parent else 0)

def resolve_base_type(base,schema):
    if base in ('INTEGER','REAL','STRING','ENUMERATION','BOOLEAN','NUMBER', 'SELECT','LOGICAL'):
        return base
    if base in schema.types:
        return resolve_base_type(schema.types[base].equals,schema)
    print(base)
    return None

def isobjref(field, schema):
    return (field.type in schema.entities)

def getvalue(field, schema):
    base = field.type
    value_str = ""

    if not isobjref(field, schema) or field.collection:
        accessor = ".Get()" if field.optional else ""

        if field.collection:
            value_str = "ListToString(in." + field.name + accessor + ")"
        else:
            real_type = resolve_base_type(field.type, schema)

            if (real_type in ["ENUMERATION", "STRING", "SELECT"]):
                value_str = "aiString(in.{name}" + accessor + ")"
            elif (real_type == 'INTEGER'):
                value_str = "(uint64_t)(in.{name}" + accessor + ")"
            else:
                value_str = "in.{name}" + accessor

    return value_str.format(name=field.name)

def gen_converter(entity,schema):
    max_arg = count_args_up(entity,schema)
    arg_idx = arg_idx_ofs = max_arg - len(entity.members)

    code = template_converter_prologue

    numProperties = sum([1 for f in entity.members if not f.optional \
            and (not isobjref(f, schema) or f.collection)]) + 1
    code += template_count_field.format(numProperties=numProperties)

    for field in entity.members:
        if field.optional and (not isobjref(field, schema) or field.collection):
            code += template_count_optional.format(name=field.name)

    code += template_alloc_strings
    code += template_ifcid

    for field in entity.members:
        if not isobjref(field, schema) or field.collection:
            if field.optional:
                code += template_set_attribute_optional.format(
                    value_str=getvalue(field, schema),
                    name=field.name)
            else:
                code += template_set_attribute.format(
                    value_str=getvalue(field, schema),
                    name=field.name)

    numChildren = sum([1 for f in entity.members if isobjref(f, schema)])

    if numChildren:
        code += template_subnodes

        for field in entity.members:
            if isobjref(field, schema) and not field.collection:
                    if field.optional:
                        code += (template_if_exists + '{{\n').format(name=field.name) +"\t"

                    code += "\t" + template_subnode_process.format(name=field.name, \
                        type=field.type, \
                        optional=(".Get()" if field.optional else ""))

                    if field.optional:
                        code += "\t}\n"

        code += template_subnode_epilogue

    return code+template_converter_epilogue

def get_base_classes(e,schema):
    def addit(e,out):
        if e.parent:
            out.append(e.parent)
            addit(schema.entities[e.parent],out)
    res = []
    addit(e,res)
    return list(reversed(res))

def get_derived(e,schema):
    def get_deriv(e,out): # bit slow, but doesn't matter here
        s = [ee for ee in schema.entities.values() if ee.parent == e.name]
        for sel in s:
            out.append(sel.name)
            get_deriv(sel,out)
    res = []
    get_deriv(e,res)
    return res

def get_hierarchy(e,schema):
    return get_derived(e.schema)+[e.name]+get_base_classes(e,schema)

def sort_entity_list(schema):
    deps = []
    entities = schema.entities
    for e in entities.values():
        deps += get_base_classes(e,schema)+[e.name]

    checked = []
    for e in deps:
        if e not in checked:
            checked.append(e)
    return [entities[e] for e in checked]

def work(filename):
    schema = ExpressReader.read(filename,silent=True)
    entities, converters, header = '','',''

    whitelist = []
    with open('metalist.txt', 'rt') as inp:
        whitelist = [n.strip() for n in inp.read().split('\n') if n[:1]!='#' and n.strip()]

    schema.whitelist = set()
    schema.blacklist_partial = set()
    for ename in whitelist:
        try:
            e = schema.entities[ename]
        except KeyError:
            # type, not entity
            continue
        for base in [e.name]:
            schema.whitelist.add(base)

    schema.blacklist_partial -= schema.whitelist
    schema.whitelist |= schema.blacklist_partial

    # uncomment this to disable automatic code reduction based on whitelisting all used entities
    # (blacklisted entities are those who are in the whitelist and may be instanced, but will
    # only be accessed through a pointer to a base-class.
    #schema.whitelist = set(schema.entities.keys())
    #schema.blacklist_partial = set()
    sorted_entities = sort_entity_list(schema)

    for entity in sorted_entities:
        parent = entity.parent+',' if entity.parent else ''

        if entity.name in schema.whitelist:
            converters += template_converter.format(type=entity.name,contents=gen_converter(entity,schema))
            header += template_header.format(type=entity.name)

    with open(input_template_h,'rt') as inp:
        with open(output_file_h,'wt') as outp:
            # can't use format() here since the C++ code templates contain single, unescaped curly brackets
            outp.write(inp.read().replace('{converter-decl}', header))

    with open(input_template_cpp,'rt') as inp:
        with open(output_file_cpp,'wt') as outp:
            outp.write(inp.read().replace('{converter-impl}',converters))

if __name__ == "__main__":
    sys.exit(work(sys.argv[1] if len(sys.argv)>1 else 'schema.exp'))





