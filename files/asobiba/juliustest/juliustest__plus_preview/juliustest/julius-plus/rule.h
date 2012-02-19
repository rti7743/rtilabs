#pragma once
#include <string>
#include <list>
#include <assert.h>
#include <algorithm>

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
	virtual ~rule()
	{
		this->Clear();
	}
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
	void RemoveNestRule(rule* haveRule)
	{
		std::remove_if(this->nestrules.begin(),this->nestrules.end() , [&](rule* r) -> bool { if (r == haveRule){ delete r; return true; }else{ return false; } } );
	}
	void RemoveWord(std::string word)
	{
		std::remove_if(this->words.begin(),this->words.end() , [&](wordnode* w) -> bool { if (w->word == word){ delete w; return true; }else{ return false; } } );
	}

	std::string getName() const
	{
		return this->name;
	}
	void Clear()
	{
		{
			for(auto it = this->words.begin() ; it != this->words.end() ; ++it)
			{
				delete *it;
			}
			this->words.clear();
		}
		{
			for(auto it = this->nestrules.begin() ; it != this->nestrules.end() ; ++it)
			{
				delete *it;
			}
			this->nestrules.clear();
		}
		delete this->combineRule;
		this->combineRule = NULL;
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
	//読みを作成する。
	std::string convertYomi(const std::string& word) const;
};

