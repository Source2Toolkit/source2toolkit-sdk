"""
Source2Toolkit
Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
All rights reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 3.0, as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
gives you permission to link the code of this program
(as well as its derivative works) to "Counter-Strike 2," "Source 2,"
"Steam," and any Game MODs or server software running on software by
Valve Corporation. You must obey the GNU General Public License in all
respects for all other code used.

Additionally, this exception applies to all derivative works unless
otherwise stated in LICENSE.txt.

Authors:
    - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl

Project: Source2Toolkit

API reference generator for source2toolkit.net.

    python generator.py <sdk>/public <website>/content/docs

Writes, under the destination:
    core-api/   one page per IToolkit*.h (+ Schema, utils/*), and meta.json
    schema/     hand-written schema structs, entity/classes, entity/enums, meta.json
Everything else in the destination is left alone. The website's sync workflow
deletes core-api/, schema/ and enums/ before running this, so the folders are
always exactly what the SDK headers say.
"""

import json
import os
import re
import shutil
import sys

import cpp_parser as P

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SOURCE_DIR = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else os.path.join(SCRIPT_DIR, '../../public'))
DEST_DIR = os.path.abspath(sys.argv[2] if len(sys.argv) > 2 else os.path.join(SCRIPT_DIR, '../../docs'))

REPO_BLOB = 'https://github.com/Source2Toolkit/source2toolkit-sdk/blob/main/public/'

# --------------------------------------------------------------------------- #
# Where every header goes
# --------------------------------------------------------------------------- #

# Titles for the hand-written schema headers (the file names are not names).
SCHEMA_TITLES = {
    'attackerinfo': 'AttackerInfo_t',
    'entityio': 'Entity I/O',
    'movedata': 'CMoveData',
    'navarea': 'Navigation mesh',
    'netmessages': 'Net message types',
    'recipientfilter': 'Recipient filters',
    'takedamageinfo': 'CTakeDamageInfo',
    'takedamageresult': 'CTakeDamageResult',
}

# The API sidebar, grouped. Anything not listed lands at the end.
API_GROUPS = [
    ('Plugin', ['IToolkitPlugin', 'IToolkitApi', 'IToolkitTypes']),
    ('Gameplay', ['IToolkitCommands', 'IToolkitPermissions', 'IToolkitConVars', 'IToolkitEvents', 'IToolkitEntities',
                  'IToolkitMenus', 'IToolkitCustomHud', 'IToolkitNetworkMessages', 'IToolkitSounds',
                  'IToolkitTransmit', 'IToolkitTrace', 'IToolkitScheduler', 'IToolkitScripts']),
    ('Engine', ['IToolkitGameHooks', 'IToolkitAddresses', 'IToolkitHooks', 'IToolkitKHook', 'IToolkitGameConfig', 'IToolkitGameSystems',
                'IToolkitMemory', 'IToolkitModule', 'Schema']),
    ('Services', ['IToolkitHTTP', 'IToolkitJSON', 'IToolkitMySQL', 'IToolkitPaths']),
]


def pascal(stem):
    return ''.join(w[:1].upper() + w[1:] for w in stem.split('-'))


def page_for(rel):
    """rel: path under public/, e.g. 'source2toolkit/IToolkitApi.h'.
    Returns (output path relative to DEST, url, kind) or None."""
    rel = rel.replace('\\', '/')
    parts = rel.split('/')
    if parts[0] == 'source2toolkit':
        parts = parts[1:]
    stem = parts[-1][:-2]

    if len(parts) == 1:
        return f'core-api/{stem}.mdx', f'/docs/core-api/{stem}', 'api'
    if parts[0] == 'utils':
        return f'core-api/utils/{pascal(stem)}.mdx', f'/docs/core-api/utils/{pascal(stem)}', 'api'
    if parts[0] == 'schema':
        if len(parts) == 2:
            if stem == 'schema':
                return 'core-api/Schema.mdx', '/docs/core-api/Schema', 'api'
            return f'schema/{pascal(stem)}.mdx', f'/docs/schema/{pascal(stem)}', 'schema'
        if parts[1] == 'entity' and len(parts) == 4:
            return (f'schema/entity/{parts[2]}/{stem}.mdx',
                    f'/docs/schema/entity/{parts[2]}/{stem}',
                    'entity-class' if parts[2] == 'classes' else 'entity-enum')
    return None


# --------------------------------------------------------------------------- #
# Markdown helpers
# --------------------------------------------------------------------------- #

TYPE_MAP = {}          # name -> url (with #anchor for things inside a page)
GLOBALS = {}           # interface class -> global pointer name


def slug(text):
    """github-slugger, which Fumadocs uses for heading ids."""
    text = text.lower()
    text = re.sub(r'[^\w\- ]', '', text)
    return text.replace(' ', '-')


_CODE_SPLIT = re.compile(r'(```.*?```|`[^`\n]*`)', re.S)


def escape_prose(text):
    """Make free text safe for MDX, leaving code spans/blocks alone."""
    out = []
    for part in _CODE_SPLIT.split(text or ''):
        if part.startswith('`'):
            if not part.startswith('```'):
                inner = part.strip('`')
                if inner in TYPE_MAP:
                    part = f'[{part}]({TYPE_MAP[inner]})'
            out.append(part)
            continue
        part = (part.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')
                .replace('{', '\\{').replace('}', '\\}'))
        out.append(part)
    return ''.join(out)


def cell(text):
    return escape_prose(text).replace('|', '\\|').replace('\n', ' ')


def type_md(typ, own_page=None):
    """A type as inline code, linked when a known type appears in it."""
    typ = (typ or '').strip()
    if not typ:
        return ''
    code = '`' + typ.replace('`', '') + '`'
    for tok in re.findall(r'[A-Za-z_]\w*', typ):
        if tok in ('const', 'unsigned', 'signed', 'struct', 'class', 'enum', 'volatile'):
            continue
        url = TYPE_MAP.get(tok)
        if url and url != own_page:
            return f'[{code}]({url})'
    return code


def doc_blocks(doc, notes=True):
    """brief + body paragraphs + notes/warnings as markdown lines."""
    lines = []
    if doc.get('deprecated'):
        lines.append(f'<Callout type="warn" title="Deprecated">{escape_prose(doc["deprecated"])}</Callout>\n')
    if doc.get('brief'):
        lines.append(escape_prose(doc['brief']) + '\n')
    for block in doc.get('body', []):
        if block.startswith('```'):
            lines.append(block + '\n')
        else:
            lines.append(escape_prose(block) + '\n')
    if notes:
        for n in doc.get('notes', []):
            lines.append(f'<Callout type="info">{escape_prose(n)}</Callout>\n')
        for w in doc.get('warnings', []):
            lines.append(f'<Callout type="warn">{escape_prose(w)}</Callout>\n')
    return lines


def frontmatter(title, description=''):
    fm = ['---', f'title: {json.dumps(title)}']
    if description:
        description = re.sub(r'`', '', description).strip()
        fm.append(f'description: {json.dumps(description)}')
    fm.append('---')
    return '\n'.join(fm) + '\n\n'


# --------------------------------------------------------------------------- #
# Rendering
# --------------------------------------------------------------------------- #

BOILERPLATE_METHODS = {'New', 'FromIndex', 'GetHandle'}


def render_functions(funcs, heading, own_page, cls_name=None):
    """Methods/functions grouped by name: one heading, every overload's signature."""
    out = []
    groups = {}
    order = []
    for f in funcs:
        if f['name'].startswith('~') or f.get('deleted'):
            continue
        if f['name'].startswith('operator'):
            continue
        groups.setdefault(f['name'], []).append(f)
        if f['name'] not in order:
            order.append(f['name'])

    for name in order:
        overloads = groups[name]
        title = name
        if cls_name and name == cls_name:
            title = f'{name} (constructor)'
        out.append(f'{heading} `{title}`\n')
        out.append('```cpp\n' + '\n'.join(f['signature'] for f in overloads) + '\n```\n')

        seen_docs = set()
        for f in overloads:
            doc = f['doc']
            key = (doc['brief'], tuple(doc['body']))
            if key in seen_docs:
                continue
            seen_docs.add(key)
            out.extend(doc_blocks(doc))

        # parameters of every overload, first description wins
        params = []
        names = set()
        descs = {}
        for f in overloads:
            descs.update({k: v for k, v in f['doc']['params'].items() if k not in descs})
            for p in f['params']:
                key = (p['name'], p['type'])
                if key in names:
                    continue
                names.add(key)
                params.append(p)
        if params:
            has_default = any(p['default'] for p in params)
            has_desc = any(descs.get(p['name']) for p in params)
            head = '| Parameter | Type |' + (' Default |' if has_default else '') + (' Description |' if has_desc else '')
            sep = '|---|---|' + ('---|' if has_default else '') + ('---|' if has_desc else '')
            out.append(head)
            out.append(sep)
            for p in params:
                row = f'| `{p["name"] or "—"}` | {type_md(p["type"], own_page)} |'
                if has_default:
                    row += f' {("`" + p["default"] + "`") if p["default"] else ""} |'
                if has_desc:
                    row += f' {cell(descs.get(p["name"], ""))} |'
                out.append(row)
            out.append('')

        rets = [f for f in overloads if f['doc']['returns']]
        if rets:
            f = rets[0]
            out.append(f'**Returns** {type_md(f["return"], own_page)} — {escape_prose(f["doc"]["returns"])}\n')
    return out


def render_fields(fields, own_page, schema=False):
    if not fields:
        return []
    has_desc = any(f['doc']['brief'] for f in fields)
    out = ['| Field | Type |' + (' Description |' if has_desc else ''),
           '|---|---|' + ('---|' if has_desc else '')]
    for f in fields:
        typ = f['type'] + (f.get('array') or '')
        row = f'| `{f["name"]}` | {type_md(typ, own_page)} |'
        if has_desc:
            row += f' {cell(f["doc"]["brief"])} |'
        out.append(row)
    out.append('')
    if schema:
        out.append('Schema fields are accessors: read with `entity->m_field()`, write with '
                   '`entity->m_field = value` (the write marks the field for networking).\n')
    return out


def render_enum(e, heading, own_page):
    out = [f'{heading} `{e["name"]}`\n'] if heading else []
    decl = 'enum ' + ('class ' if e['is_class'] else '') + e['name'] + \
           (f' : {e["underlying"]}' if e['underlying'] else '')
    out.append('```cpp\n' + decl + '\n```\n')
    out.extend(doc_blocks(e['doc']))
    if e['values']:
        has_val = any(v['value'] for v in e['values'])
        has_desc = any(v['doc']['brief'] for v in e['values'])
        out.append('| Name |' + (' Value |' if has_val else '') + (' Description |' if has_desc else ''))
        out.append('|---|' + ('---|' if has_val else '') + ('---|' if has_desc else ''))
        for v in e['values']:
            row = f'| `{v["name"]}` |'
            if has_val:
                row += f' {("`" + v["value"] + "`") if v["value"] else ""} |'
            if has_desc:
                row += f' {cell(v["doc"]["brief"])} |'
            out.append(row)
        out.append('')
    return out


def render_typedefs(tds, heading, own_page):
    out = []
    seen = set()
    for t in tds:
        decls = [x['decl'] for x in tds if x['name'] == t['name']]
        if t['name'] in seen:
            continue
        seen.add(t['name'])
        out.append(f'{heading} `{t["name"]}`\n')
        out.append('```cpp\n' + '\n'.join(dict.fromkeys(decls)) + '\n```\n')
        docs = [x['doc'] for x in tds if x['name'] == t['name'] and not P.is_empty_doc(x['doc'])]
        if docs:
            out.extend(doc_blocks(docs[0]))
        if len(decls) > 1:
            out.append('<Callout type="info">Declared differently per platform (`#ifdef _WIN32`); '
                       'the first line is the Windows form, the second the Linux one.</Callout>\n')
    return out


def render_macros(macros, own_page):
    shown = [m for m in macros if not m['name'].endswith('_INTERFACE')
             and (m['params'] or not P.is_empty_doc(m['doc']))]
    if not shown:
        return []
    out = ['## Macros\n']
    documented = [m for m in shown if not P.is_empty_doc(m['doc'])]
    plain = [m for m in shown if P.is_empty_doc(m['doc'])]
    for m in documented:
        out.append(f'### `{m["name"]}`\n')
        value = m['value']
        if len(value) > 400:
            value = value[:400].rsplit('\n', 1)[0] + '\n    ...'
        out.append('```cpp\n#define ' + m['name'] + m['params'] + (' \\\n    ' + value if '\n' in value else (' ' + value if value else '')) + '\n```\n')
        out.extend(doc_blocks(m['doc']))
    if plain:
        if documented:
            out.append('### Other macros\n')
        out.append('| Macro | Expands to |')
        out.append('|---|---|')
        for m in plain:
            value = re.sub(r'\s+', ' ', m['value'])
            if len(value) > 110:
                value = value[:107] + '...'
            out.append(f'| `{m["name"]}{m["params"]}` | `{value.replace("|", "∣").replace("`", "")}` |')
        out.append('')
    return out


def render_record(cls, level, own_page, primary):
    """A class/struct. level 2: '##' for the class, '###' for members."""
    h_cls = '#' * level
    h_mem = '#' * (level + 1)
    out = []
    if not primary:
        out.append(f'{h_cls} `{cls["name"]}`\n')
        out.extend(doc_blocks(cls['doc']))
    if cls['bases']:
        out.append('**Inherits** ' + ', '.join(type_md(b, own_page) for b in cls['bases']) + '\n')

    fields = cls['schema_fields'] or cls['fields']
    methods = [m for m in cls['methods']
               if not (cls['schema_class'] and cls['name'] != 'CBaseEntity'
                       and m['name'] in BOILERPLATE_METHODS and P.is_empty_doc(m['doc']))]

    if fields:
        out.append(f'{h_mem if not primary else "##"} Fields\n')
        out.extend(render_fields(fields, own_page, schema=bool(cls['schema_fields'])))
    ctors = [m for m in methods if m.get('is_ctor') and not m['name'].startswith('~')]
    others = [m for m in methods if not m.get('is_ctor')]
    if ctors:
        out.append(f'{h_mem if not primary else "##"} Constructors\n')
        out.append('```cpp\n' + '\n'.join(c['signature'] for c in ctors) + '\n```\n')
        for c in ctors:
            if not P.is_empty_doc(c['doc']):
                out.extend(doc_blocks(c['doc']))
    if others:
        if primary:
            out.append('## Methods\n')
            out.extend(render_functions(others, '###', own_page, cls['name']))
        else:
            out.extend(render_functions(others, h_mem, own_page, cls['name']))
    for e in cls['enums']:
        out.extend(render_enum(e, h_mem, own_page))
    if cls['typedefs']:
        out.extend(render_typedefs(cls['typedefs'], h_mem, own_page))
    for n in cls['nested']:
        if n['fields'] or n['methods'] or n['schema_fields']:
            out.extend(render_record(n, min(level + 1, 3), own_page, False))
    return out


def api_page(rel, parsed, url, title):
    stem = os.path.basename(rel)[:-2]
    classes = [c for c in parsed['classes']
               if c['fields'] or c['methods'] or c['schema_fields'] or not P.is_empty_doc(c['doc'])]
    primary = next((c for c in classes if c['name'] == title), None)
    if primary is None and len(classes) == 1:
        primary = classes[0]

    fdoc = parsed['file_doc']
    description = fdoc['brief'] or (primary['doc']['brief'] if primary else '')
    if not description and rel.replace('\\', '/').split('/')[-2:-1] == ['schema']:
        names = ', '.join(c['name'] for c in parsed['classes'][:4]) or title
        description = f'Engine structures the SDK declares by hand: {names}.'
    out = [frontmatter(title, description)]

    body_doc = dict(fdoc)
    body_doc['brief'] = ''
    out.extend(doc_blocks(body_doc))
    if primary and not P.is_empty_doc(primary['doc']) and primary['doc']['brief'] != description:
        out.extend(doc_blocks(primary['doc']))
    elif primary:
        pdoc = dict(primary['doc'])
        pdoc['brief'] = ''
        out.extend(doc_blocks(pdoc))

    # the at-a-glance table
    info = []
    for macro, iid in parsed['interfaces'].items():
        info.append(('Interface id', f'`{iid}` (`{macro}`)'))
    if primary and primary['name'] in GLOBALS:
        info.append(('Global', f'`{GLOBALS[primary["name"]]}`'))
    header = rel.replace('\\', '/')
    info.append(('Header', f'[`{header}`]({REPO_BLOB}{header})'))
    out.append(' · '.join(f'**{k}** {v}' for k, v in info) + '\n')

    if parsed['typedefs']:
        out.append('## Types\n')
        out.extend(render_typedefs(parsed['typedefs'], '###', url))

    # a getter returning a documented alias (IToolkitAddresses: X_t X()) borrows its doc
    td_docs = {t['name']: t['doc'] for t in parsed['typedefs'] if not P.is_empty_doc(t['doc'])}
    for c in classes:
        for m in c['methods']:
            if P.is_empty_doc(m['doc']) and m['return'] in td_docs:
                borrowed = dict(td_docs[m['return']])
                borrowed['body'] = []
                borrowed['notes'] = []
                borrowed['warnings'] = []
                m['doc'] = borrowed

    if primary:
        out.extend(render_record(primary, 2, url, True))
    for c in classes:
        if c is not primary:
            out.extend(render_record(c, 2, url, False))

    if parsed['enums']:
        out.append('## Enums\n')
        for e in parsed['enums']:
            out.extend(render_enum(e, '###', url))

    if parsed['functions']:
        out.append('## Functions\n')
        out.extend(render_functions(parsed['functions'], '###', url))

    out.extend(render_macros(parsed['macros'], url))
    return '\n'.join(out).rstrip() + '\n'


def entity_class_page(rel, parsed, url):
    cls = next((c for c in parsed['classes'] if c['schema_class'] or c['schema_fields']), None) \
        or (parsed['classes'][0] if parsed['classes'] else None)
    if cls is None:
        return None
    base = cls['bases'][0] if cls['bases'] else ''
    base_clean = re.sub(r'CBaseEntity::Factory<.*>', 'CBaseEntity', base)
    description = f'Schema class {cls["name"]}' + (f', derived from {base_clean}.' if base_clean else '.')
    out = [frontmatter(cls['name'], description)]
    if cls['bases']:
        cls = dict(cls)
        cls['bases'] = [base_clean] + cls['bases'][1:]
    header = rel.replace('\\', '/')
    out.append(f'Generated from the game\'s schema. Header: [`{header}`]({REPO_BLOB}{header})\n')
    out.extend(render_record(cls, 2, url, True))
    for c in parsed['classes']:
        if c is not cls and c['name'] != cls['name'] and (c['fields'] or c['schema_fields']):
            out.extend(render_record(c, 2, url, False))
    return '\n'.join(out).rstrip() + '\n'


def entity_enum_page(rel, parsed, url):
    if not parsed['enums']:
        return None
    e = parsed['enums'][0]
    out = [frontmatter(e['name'], f'Schema enum {e["name"]} ({len(e["values"])} values).')]
    out.extend(render_enum(e, None, url))
    for other in parsed['enums'][1:]:
        out.extend(render_enum(other, '##', url))
    return '\n'.join(out).rstrip() + '\n'


# --------------------------------------------------------------------------- #
# Navigation
# --------------------------------------------------------------------------- #

def write_json(path, obj):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        json.dump(obj, f, indent=2, ensure_ascii=False)
        f.write('\n')


def write_navigation(api_pages, schema_pages):
    listed = set()
    pages = ['index']
    for title, names in API_GROUPS:
        present = [n for n in names if n in api_pages]
        if present:
            pages.append(f'---{title}---')
            pages.extend(present)
            listed.update(present)
    rest = sorted(n for n in api_pages if n not in listed)
    if rest:
        pages.append('---Other---')
        pages.extend(rest)
    pages.append('utils')
    write_json(os.path.join(DEST_DIR, 'core-api', 'meta.json'), {
        'title': 'API', 'description': 'Interfaces, types and macros of the SDK', 'root': True,
        'icon': 'Braces', 'pages': pages})
    write_json(os.path.join(DEST_DIR, 'core-api', 'utils', 'meta.json'), {'title': 'Utilities'})

    write_json(os.path.join(DEST_DIR, 'schema', 'meta.json'), {
        'title': 'Schema', 'description': 'Entity classes, enums and engine structures', 'root': True,
        'icon': 'Database',
        'pages': ['index', '---Structures---'] + sorted(schema_pages, key=str.lower)
                 + ['---Entity system---', 'entity']})
    write_json(os.path.join(DEST_DIR, 'schema', 'entity', 'meta.json'),
               {'title': 'Entity system', 'defaultOpen': True, 'pages': ['classes', 'enums']})
    write_json(os.path.join(DEST_DIR, 'schema', 'entity', 'classes', 'meta.json'), {'title': 'Classes'})
    write_json(os.path.join(DEST_DIR, 'schema', 'entity', 'enums', 'meta.json'), {'title': 'Enums'})


def copy_static_pages():
    src_root = os.path.join(SCRIPT_DIR, 'pages')
    if not os.path.isdir(src_root):
        return
    for root, _, files in os.walk(src_root):
        for f in files:
            src = os.path.join(root, f)
            dst = os.path.join(DEST_DIR, os.path.relpath(src, src_root))
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copy2(src, dst)


# --------------------------------------------------------------------------- #
# Main
# --------------------------------------------------------------------------- #

def main():
    headers = []
    for root, _, files in os.walk(SOURCE_DIR):
        for f in sorted(files):
            if f.endswith('.h'):
                full = os.path.join(root, f)
                headers.append(os.path.relpath(full, SOURCE_DIR).replace('\\', '/'))
    headers.sort()

    parsed = {}
    for rel in headers:
        target = page_for(rel)
        if not target:
            continue
        with open(os.path.join(SOURCE_DIR, rel), encoding='utf-8-sig') as fh:
            parsed[rel] = P.parse_header(fh.read())

    # globals a plugin gets (g_pToolkitX) from TOOLKIT_DEFINE_GLOBALVARS
    plugin_h = next((r for r in parsed if r.endswith('IToolkitPlugin.h')), None)
    if plugin_h:
        text = open(os.path.join(SOURCE_DIR, plugin_h), encoding='utf-8-sig').read()
        for cls, glob in re.findall(r'\b(IToolkit\w+)\*\s+(g_pToolkit\w+)\s*=', text):
            GLOBALS[cls] = glob
        GLOBALS.setdefault('IToolkitAPI', 'g_ToolkitAPI')

    # first pass: every documented name -> url
    for rel, r in parsed.items():
        _, url, kind = page_for(rel)
        stem = os.path.basename(rel)[:-2]
        for c in r['classes']:
            if kind in ('entity-class',) or c['name'] == stem or len(r['classes']) == 1:
                TYPE_MAP.setdefault(c['name'], url)
            else:
                TYPE_MAP.setdefault(c['name'], f'{url}#{slug(c["name"])}')
        for e in r['enums']:
            TYPE_MAP.setdefault(e['name'], url if kind == 'entity-enum' else f'{url}#{slug(e["name"])}')
        for t in r['typedefs']:
            TYPE_MAP.setdefault(t['name'], f'{url}#{slug(t["name"])}')

    # clean the generated folders
    for d in ('core-api', 'schema', 'enums'):
        shutil.rmtree(os.path.join(DEST_DIR, d), ignore_errors=True)

    api_pages, schema_pages = [], []
    written = 0
    for rel, r in parsed.items():
        out_rel, url, kind = page_for(rel)
        stem = os.path.basename(rel)[:-2]
        if kind == 'entity-class':
            content = entity_class_page(rel, r, url)
        elif kind == 'entity-enum':
            content = entity_enum_page(rel, r, url)
        else:
            if kind == 'schema':
                title = SCHEMA_TITLES.get(stem, pascal(stem))
            elif stem == 'schema':
                title = 'Schema'
            elif out_rel.startswith('core-api/utils/'):
                title = f'utils/{stem}.h'
            else:
                title = stem
            has = r['classes'] or r['enums'] or r['typedefs'] or r['functions'] or \
                [m for m in r['macros'] if m['params'] or not P.is_empty_doc(m['doc'])]
            content = api_page(rel, r, url, title) if has else None
            if content:
                if kind == 'schema':
                    schema_pages.append(os.path.basename(out_rel)[:-4])
                elif not out_rel.startswith('core-api/utils/'):
                    api_pages.append(os.path.basename(out_rel)[:-4])
        if not content:
            continue
        path = os.path.join(DEST_DIR, out_rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w', encoding='utf-8', newline='\n') as fh:
            fh.write(content)
        written += 1

    copy_static_pages()
    write_navigation(api_pages, schema_pages)
    print(f'Wrote {written} pages ({len(api_pages)} API, {len(schema_pages)} schema structures) to {DEST_DIR}')


if __name__ == '__main__':
    main()
