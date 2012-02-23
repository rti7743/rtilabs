#pragma once
#include <string>
#include <list>
#include <assert.h>
#include <algorithm>
#include <functional>

struct CallbackDataStruct
{
	CallbackDataStruct( )
	{
		Enable = false;
//		callbackFunctionalObject = [](){};
	}
	CallbackDataStruct( std::function<void (void)> f)  
	{
		Enable = true;
		callbackFunctionalObject = f;
	}
	CallbackDataStruct( const CallbackDataStruct& copy)
	{
		this->Enable = copy.Enable;
		this->callbackFunctionalObject = copy.callbackFunctionalObject;
	}

	bool Enable;
	std::function<void (void)> callbackFunctionalObject;
};


class JuliusPlusRule
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
	std::list<JuliusPlusRule*> nestrules;
	JuliusPlusRule* combineRule;
	CallbackDataStruct callback;

public:

	JuliusPlusRule(const std::string& w,const CallbackDataStruct& c) : id(INT_MAX) , name(w) , combineRule(NULL),callback(c)
	{
	}
	JuliusPlusRule(int id,const CallbackDataStruct& c) : id(id), combineRule(NULL) ,callback(c)
	{
	}
	virtual ~JuliusPlusRule()
	{
		this->Clear();
	}
	bool AddWord(const std::string& w )
	{
		this->words.push_back(new wordnode(w,convertYomi(w) ) );
		return true;
	}
	bool AddWord(const std::string& w , const std::string& yomi )
	{
		this->words.push_back(new wordnode(w,yomi) );
		return true;
	}
	bool AddNestRule(JuliusPlusRule* haveRule)
	{
		this->nestrules.push_back(haveRule);
		return true;
	}
	bool SetCombineRule(JuliusPlusRule* combineRule)
	{
		assert(this->combineRule == NULL);
		this->combineRule = combineRule;
		return true;
	}
	void RemoveNestRule(JuliusPlusRule* haveRule)
	{
		std::remove_if(this->nestrules.begin(),this->nestrules.end() , [&](JuliusPlusRule* r) -> bool { if (r == haveRule){ delete r; return true; }else{ return false; } } );
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
	const std::list<JuliusPlusRule*>* getNestRules() const
	{
		return &this->nestrules;
	}
	const JuliusPlusRule* getTermRule() const
	{
		return this->combineRule;
	}
	const CallbackDataStruct* getCallback() const
	{
		return &this->callback;
	}
	//読みを作成する。
	static std::string convertYomi(const std::string& word) ;
};

