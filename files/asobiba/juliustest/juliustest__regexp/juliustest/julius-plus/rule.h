#pragma once
#include <string>
#include <list>
#include <assert.h>

class rule
{
	std::string name;
	int id;

	struct wordnode
	{
		wordnode(const std::string& word,const std::string& yomi) : word(word) , yomi(yomi){};

		std::string word;
		std::string yomi;
	};
	std::list<wordnode*> words;
	std::list<rule*> nestrules;
	rule* combineRule;

public:
	rule(const std::string& w) : id(INT_MAX) , name(w) , combineRule(NULL) 
	{
	}
	rule(int id) : id(id), combineRule(NULL)
	{
	}
	virtual ~rule();
	bool AddWord(const std::string& w , const std::string& yomi = "")
	{
		if (yomi.empty())
		{
			this->words.push_back(new wordnode(w,convertYomi(w) ) );
		}
		else
		{
			this->words.push_back(new wordnode(w,yomi) );
		}
		return true;
	}
	bool AddNestRule(rule* haveRule)
	{
		this->nestrules.push_back(haveRule);
		return true;
	}
	bool SetCombineRule(rule* combineRule)
	{
		assert(this->combineRule == NULL);
		this->combineRule = combineRule;
		return true;
	}

	std::string getRuleName() const;
	const std::list<wordnode*>* getWords() const
	{
		return &this->words;
	}
	const std::list<rule*>* getNestRules() const
	{
		return &this->nestrules;
	}
	const rule* getTermRule() const
	{
		return this->combineRule;
	}
	bool Commit(const std::string & parentRuleName ,std::ostringstream* grammar , std::ostringstream* voca  );
	//“Ç‚İ‚ğì¬‚·‚éB
	std::string convertYomi(const std::string& word) const;
};

