#include "duckdb/function/table/system_functions.hpp"

#include "duckdb/common/algorithm.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/parser/parser.hpp"
#include "duckdb/parser/peg/compiled_grammar.hpp"
#include "duckdb/parser/peg/keyword_helper/duckdb_keyword_helper.hpp"

namespace duckdb {

struct DuckDBKeywordsData : public GlobalTableFunctionState {
	DuckDBKeywordsData() : offset(0) {
	}

	vector<ParserKeyword> entries;
	idx_t offset;
};

static unique_ptr<FunctionData> DuckDBKeywordsBind(ClientContext &context, TableFunctionBindInput &input,
                                                   vector<LogicalType> &return_types, vector<Identifier> &names) {
	names.emplace_back("keyword_name");
	return_types.emplace_back(LogicalType::VARCHAR);

	names.emplace_back("keyword_category");
	return_types.emplace_back(LogicalType::VARCHAR);

	return nullptr;
}

//! Grammar extensions mostly add words only as literals in their rules, without a keyword category. Those words are
//! still accepted as identifiers, so they are reported as unreserved. The base grammar's literal-only words are left
//! out, so the built-in part of the list stays the same as without grammar extensions.
static vector<ParserKeyword> ExtendedGrammarKeywords(const CompiledGrammar &grammar) {
	auto &keyword_helper = grammar.GetKeywordHelper();
	auto &base_keyword_helper = DuckDBKeywordHelper::Instance();
	auto result = keyword_helper.KeywordList();
	vector<ParserKeyword> literal_words;
	keyword_helper.GetLiteralTable().ForEachLiteral([&](const string &text, const LiteralInfo &info) {
		if (info.IsKeyword() || !keyword_helper.IsGrammarWord(text) || base_keyword_helper.IsGrammarWord(text)) {
			return;
		}
		literal_words.push_back({StringUtil::Lower(text), "unreserved"});
	});
	std::sort(literal_words.begin(), literal_words.end(),
	          [](const ParserKeyword &a, const ParserKeyword &b) { return a.name < b.name; });
	result.insert(result.end(), literal_words.begin(), literal_words.end());
	return result;
}

unique_ptr<GlobalTableFunctionState> DuckDBKeywordsInit(ClientContext &context, TableFunctionInitInput &input) {
	auto result = make_uniq<DuckDBKeywordsData>();
	auto grammar = CompiledGrammar::Get(context);
	if (grammar == CompiledGrammar::DefaultGrammar()) {
		result->entries = Parser::KeywordList();
	} else {
		result->entries = ExtendedGrammarKeywords(*grammar);
	}
	return std::move(result);
}

void DuckDBKeywordsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = data_p.global_state->Cast<DuckDBKeywordsData>();
	if (data.offset >= data.entries.size()) {
		// finished returning values
		return;
	}
	// start returning values
	// either fill up the chunk or return all the remaining columns
	idx_t count = 0;

	// keyword_name, VARCHAR
	auto &keyword_name = output.data[0];
	// keyword_category, VARCHAR
	auto &keyword_category = output.data[1];

	while (data.offset < data.entries.size() && count < STANDARD_VECTOR_SIZE) {
		auto &entry = data.entries[data.offset++];

		keyword_name.Append(Value(entry.name));
		keyword_category.Append(Value(entry.category));

		count++;
	}
}

void DuckDBKeywordsFun::RegisterFunction(BuiltinFunctions &set) {
	TableFunction duckdb_keywords("duckdb_keywords", {}, DuckDBKeywordsFunction, DuckDBKeywordsBind,
	                              DuckDBKeywordsInit);
	set.AddFunction(duckdb_keywords);
}

} // namespace duckdb
