/* Copyright 2017 R. Thomas
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "LIEF/visitors/elf_json.hpp"
#include "LIEF/ELF.hpp"
namespace LIEF {
namespace ELF {

void JsonVisitor::visit(const Binary& binary) {
  JsonVisitor header_visitor;
  header_visitor(binary.get_header());

  // Sections
  std::vector<json> sections;
  for (const Section& section : binary.get_sections()) {
    JsonVisitor visitor;
    visitor(section);
    sections.emplace_back(visitor.get());
  }

  // Segments
  std::vector<json> segments;
  for (const Segment& segment : binary.get_segments()) {
    JsonVisitor visitor;
    visitor(segment);
    segments.emplace_back(visitor.get());
  }

  // Dynamic entries
  std::vector<json> dynamic_entries;
  for (const DynamicEntry& entry : binary.get_dynamic_entries()) {
    JsonVisitor visitor;
    entry.accept(visitor);
    dynamic_entries.emplace_back(visitor.get());
  }


  // Dynamic symbols
  std::vector<json> dynamic_symbols;
  for (const Symbol& symbol : binary.get_dynamic_symbols()) {
    JsonVisitor visitor;
    visitor(symbol);
    dynamic_symbols.emplace_back(visitor.get());
  }


  // Static symbols
  std::vector<json> static_symbols;
  for (const Symbol& symbol : binary.get_static_symbols()) {
    JsonVisitor visitor;
    visitor(symbol);
    static_symbols.emplace_back(visitor.get());
  }


  // Dynamic relocations
  std::vector<json> dynamic_relocations;
  for (const Relocation& reloc : binary.get_dynamic_relocations()) {
    JsonVisitor visitor;
    visitor(reloc);
    dynamic_relocations.emplace_back(visitor.get());
  }


  // pltgot relocations
  std::vector<json> pltgot_relocations;
  for (const Relocation& reloc : binary.get_pltgot_relocations()) {
    JsonVisitor visitor;
    visitor(reloc);
    pltgot_relocations.emplace_back(visitor.get());
  }


  // Symbol version
  std::vector<json> symbols_version;
  for (const SymbolVersion& s : binary.get_symbols_version()) {
    JsonVisitor visitor;
    visitor(s);
    symbols_version.emplace_back(visitor.get());
  }


  // Symbols version requirement
  std::vector<json> symbols_version_requirement;
  for (const SymbolVersionRequirement& s : binary.get_symbols_version_requirement()) {
    JsonVisitor visitor;
    visitor(s);
    symbols_version_requirement.emplace_back(visitor.get());
  }


  // Symbols version definition
  std::vector<json> symbols_version_definition;
  for (const SymbolVersionDefinition& s : binary.get_symbols_version_definition()) {
    JsonVisitor visitor;
    visitor(s);
    symbols_version_definition.emplace_back(visitor.get());
  }



  this->node_["name"]         = binary.name();
  this->node_["entrypoint"]   = binary.entrypoint();
  this->node_["imagebase"]    = binary.get_imagebase();
  this->node_["virtual_size"] = binary.get_virtual_size();
  this->node_["is_pie"]       = binary.is_pie();

  if (binary.has_interpreter()) {
    this->node_["interpreter"] = binary.get_interpreter();
  }

  this->node_["header"]                      = header_visitor.get();
  this->node_["sections"]                    = sections;
  this->node_["segments"]                    = segments;
  this->node_["dynamic_entries"]             = dynamic_entries;
  this->node_["dynamic_symbols"]             = dynamic_symbols;
  this->node_["static_symbols"]              = static_symbols;
  this->node_["dynamic_relocations"]         = dynamic_relocations;
  this->node_["pltgot_relocations"]          = pltgot_relocations;
  this->node_["symbols_version"]             = symbols_version;
  this->node_["symbols_version_requirement"] = symbols_version_requirement;
  this->node_["symbols_version_definition"]  = symbols_version_definition;
}


void JsonVisitor::visit(const Header& header) {
  this->node_["file_type"]                       = to_string(header.file_type());
  this->node_["machine_type"]                    = to_string(header.machine_type());
  this->node_["object_file_version"]             = to_string(header.object_file_version());
  this->node_["entrypoint"]                      = header.entrypoint();
  this->node_["program_headers_offset"]          = header.program_headers_offset();
  this->node_["section_headers_offset"]          = header.section_headers_offset();
  this->node_["processor_flag"]                  = header.processor_flag();
  this->node_["header_size"]                     = header.header_size();
  this->node_["program_header_size"]             = header.program_header_size();
  this->node_["processornumberof_segments_flag"] = header.numberof_segments();
  this->node_["sizeof_section_header"]           = header.sizeof_section_header();
  this->node_["numberof_sections"]               = header.numberof_sections();
  this->node_["section_name_table_idx"]          = header.section_name_table_idx();
  this->node_["identity_class"]                  = to_string(header.identity_class());
  this->node_["identity_data"]                   = to_string(header.identity_data());
  this->node_["identity_version"]                = to_string(header.identity_version());
  this->node_["identity_os_abi"]                 = to_string(header.identity_os_abi());
}


void JsonVisitor::visit(const Section& section) {
  std::vector<json> flags;
  for (SECTION_FLAGS f : section.flags_list()) {
    flags.emplace_back(to_string(f));
  }

  this->node_["name"]            = section.name();
  this->node_["virtual_address"] = section.virtual_address();
  this->node_["size"]            = section.size();
  this->node_["offset"]          = section.offset();
  this->node_["alignment"]       = section.alignment();
  this->node_["information"]     = section.information();
  this->node_["entry_size"]      = section.entry_size();
  this->node_["link"]            = section.link();
  this->node_["type"]            = to_string(section.type());
  this->node_["flags"]           = flags;
}

void JsonVisitor::visit(const Segment& segment) {

  std::vector<json> sections;
  for (const Section& section : segment.sections()) {
    sections.emplace_back(section.name());
  }

  // TODO: sections
  this->node_["type"]             = to_string(segment.type());
  this->node_["flag"]             = segment.flag();
  this->node_["file_offset"]      = segment.file_offset();
  this->node_["virtual_address"]  = segment.virtual_address();
  this->node_["physical_address"] = segment.physical_address();
  this->node_["physical_size"]    = segment.physical_size();
  this->node_["virtual_size"]     = segment.virtual_size();
  this->node_["alignment"]        = segment.alignment();
  this->node_["sections"]         = sections;

}

void JsonVisitor::visit(const DynamicEntry& entry) {
  this->node_["tag"]   = to_string(entry.tag());
  this->node_["value"] = entry.value();

}


void JsonVisitor::visit(const DynamicEntryArray& entry) {
  this->node_["array"] = entry.array();
}


void JsonVisitor::visit(const DynamicEntryLibrary& entry) {
  this->node_["library"] = entry.name();
}


void JsonVisitor::visit(const DynamicEntryRpath& entry) {
  this->node_["rpath"] = entry.rpath();
}


void JsonVisitor::visit(const DynamicEntryRunPath& entry) {
  this->node_["runpath"] = entry.runpath();
}


void JsonVisitor::visit(const DynamicSharedObject& entry) {
  this->node_["library"] = entry.name();
}

void JsonVisitor::visit(const Symbol& symbol) {
  this->node_["type"]        = to_string(symbol.type());
  this->node_["binding"]     = to_string(symbol.binding());
  this->node_["information"] = symbol.information();
  this->node_["other"]       = symbol.other();
  this->node_["value"]       = symbol.value();
  this->node_["size"]        = symbol.size();
  this->node_["name"]        = symbol.name();

  try {
    this->node_["demangled_name"] = symbol.demangled_name();
  } catch (const not_supported&) {
  }
}

void JsonVisitor::visit(const Relocation& relocation) {
  std::string relocation_type = "NOT_TO_STRING";
  std::string symbol_name     = "";

  if (relocation.has_symbol()) {
    const Symbol& s = relocation.symbol();
    try {
      symbol_name = s.demangled_name();
    } catch(const not_supported&) {
      symbol_name = s.name();
    }
  }


  if (relocation.architecture() == ARCH::EM_X86_64) {
    relocation_type = to_string(static_cast<RELOC_x86_64>(relocation.type()));
  }

  this->node_["symbol_name"] = symbol_name;
  this->node_["address"]     = relocation.address();
  this->node_["type"]        = relocation_type;

}

void JsonVisitor::visit(const SymbolVersion& sv) {
  this->node_["value"] = sv.value();
  if (sv.has_auxiliary_version()) {
   this->node_["symbol_version_auxiliary"] = sv.symbol_version_auxiliary().name();
  }

}

void JsonVisitor::visit(const SymbolVersionRequirement& svr) {

  std::vector<json> svar_json;

  for (const SymbolVersionAuxRequirement& svar : svr.get_auxiliary_symbols()) {
    JsonVisitor visitor;
    visitor(svar);
    svar_json.emplace_back(visitor.get());
  }

  this->node_["version"]                              = svr.version();
  this->node_["name"]                                 = svr.name();
  this->node_["symbol_version_auxiliary_requirement"] = svar_json;

}

void JsonVisitor::visit(const SymbolVersionDefinition& svd) {

  std::vector<json> sva_json;

  for (const SymbolVersionAux& sva : svd.symbols_aux()) {
    JsonVisitor visitor;
    visitor(sva);
    sva_json.emplace_back(visitor.get());
  }

  this->node_["version"]                  = svd.version();
  this->node_["flags"]                    = svd.flags();
  this->node_["hash"]                     = svd.hash();
  this->node_["symbol_version_auxiliary"] = sva_json;
}

void JsonVisitor::visit(const SymbolVersionAux& sv) {
  this->node_["name"] = sv.name();
}

void JsonVisitor::visit(const SymbolVersionAuxRequirement& svar) {
  this->node_["hash"]  = svar.hash();
  this->node_["flags"] = svar.flags();
  this->node_["other"] = svar.other();
}


// LIEF Abstract
void JsonVisitor::visit(const LIEF::Binary& binary) {
  // It should be a ELF::Binary so we don't catch "std::bad_cast"
  this->visit(*dynamic_cast<const LIEF::ELF::Binary*>(&binary));
}


void JsonVisitor::visit(const LIEF::Symbol& symbol) {
  // It should be a ELF::Binary so we don't catch "std::bad_cast"
  this->visit(*dynamic_cast<const LIEF::ELF::Symbol*>(&symbol));
}


void JsonVisitor::visit(const LIEF::Section& section) {
  // It should be a ELF::Binary so we don't catch "std::bad_cast"
  this->visit(*dynamic_cast<const LIEF::ELF::Section*>(&section));
}

} // namespace ELF
} // namespace LIEF
