#include "JuliusPlus.h"

#include <sstream>
template<typename _INT> static const std::string num2str(_INT i)
{
	std::stringstream out;
	out << i;
	return out.str();
}
template<typename _INT> static const std::wstring num2wstr(_INT i)
{
	std::wstringstream out;
	out << i;
	return out.str();
}

JuliusPlus::JuliusPlus() : Grammer("topgrammer")
{
	this->LocalCaptureRuleNodeCount = 0;
	this->GlobalRuleNodeCount = 0;
}

JuliusPlus::~JuliusPlus()
{
}

bool JuliusPlus::RuleCommit(std::ostream* dfa , std::ostream* dict  )
{
	struct reverse_automaton
	{
		std::ostream* dfa;
		std::ostream* dict;

		int dfaNumber ;
		int dictNumber ;

		void fire(const rule* toprule,std::ostream* dfa , std::ostream* dict)
		{
			this->dfaNumber = 3;
			this->dictNumber = 2;
			this->dfa = dfa;
			this->dict = dict;

			int firstRuleNumber = this->dfaNumber++; //3
			assert(firstRuleNumber == 3); //0 silE
			                              //1 silB
			                              //2 -1 -1
			                              //なので3からスタート

			*dfa << 0 << " " << 0 << " " << firstRuleNumber << " " << 0 << " " << 1 << std::endl;
			*dict << 0 << "\t" << "[</s>]" << "\t" << " " << "silE" << std::endl;

			*dfa << 1 << " " << 1 << " " << 2 << " " << 1 << " " << 1 << std::endl;
			*dict << 1 << "\t" << "[<s>]" << "\t" << " " << "silB" << std::endl;

			*dfa << 2 << " " << -1 << " " << -1 << " " << 1 << " " << 0 << std::endl;

			func(toprule,1,0 , firstRuleNumber);
		}

		//rule   処理するルール
		//number ルールに割り振られるべき番号 dfaNumber 番号。
		//backNumber このルールの親ルールの番号
		//termNumber このルールがどこにも遷移しない終端ルールだった時に進む番号
		void func(const rule* currentrule,int number,int backNumber,int termNumber)
		{
			assert(currentrule);
			auto words = currentrule->getWords();
			auto nestrules = currentrule->getNestRules();
			auto termRule = currentrule->getTermRule();

			if (words->size() <= 0)
			{//単語を伴わないノードだったら
				if (termRule == NULL)
				{//ネストだけのノードだったら自分をスキップする。
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , number ,backNumber , termNumber);
					}
				}
				else
				{//term ruleがあるのでそっちを実効する.
					func( termRule , number ,backNumber,termNumber);

					int nextNumber = this->dfaNumber ++ ;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
				//スキップするのでココでおわり.
				return ;
			}

			//単語辞書 dictを構築する。
			int dictNumber = this->dictNumber++;
			{
				for(auto it = words->begin(); it != words->end() ; ++it )
				{
					*dict << dictNumber << "\t" << "[" << (*it)->word << "]" << "\t" << " " << (*it)->yomi << std::endl;
				}
			}
			
			//term ruleがあれば先に実行する。
			if (termRule == NULL)
			{//終端ノードなし
				if (nestrules->size() <= 0 && words->size() >= 1)
				{//単語だけのノードの場合、終端になる。
					*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
				}
				else
				{//単語とネストが共にある場合。
				 //ネストの処理を行う。
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
			}
			else
			{
				int nestTermNumber = this->dfaNumber ++ ;

				if (nestrules->size() <= 0 && words->size() >= 1)
				{//単語だけのノードの場合、終端になる。
					*dfa << nestTermNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
				}
				else
				{//単語とネストが共にある場合。
				 //ネストの処理を行う。
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , nestTermNumber);
					}
				}
				func( termRule , nestTermNumber ,number,termNumber);
			}


		}
	} maton;
	maton.fire( &this->Grammer ,dfa , dict  );

	return true;
}

//音声認識ルールを構築します。 正規表現にも対応しています。
xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) 
{
	_USE_WINDOWS_ENCODING;

	//一番最初だけ正規表現の構文変換をかける.
    // .+ --> (:?.*)
    // (まる|さんかく)? --> (まる|さんかく|)   正しい正規表現としてはエラーだが、このエンジンの場合容認する.
    // なのは? --> なの(は|)
	std::wstring optstr = L"";

	//処理を簡単にするために、UTF-16でループします。
	for( const wchar_t * p = _A2W( str.c_str() ) ; *p ; ++p )
	{
        if ( *p == L'.' && *(p+1) == L'+')
        { // .+ --> (:?.*)
            optstr += L"(?:.+)";
			++p;
        }
        else if (*p == L'(' && *(p+1) == L'?' && *(p+2) == L':' )
		{
            optstr += L"(?:";
			p+=2;
		}
        else if (*(p+1) == L'?')
        {
            if (*p == L')')
            {// (まる|さんかく)? --> (まる|さんかく|)
                    optstr += L"|)";
            }
            else 
            {// なのは? --> なの(は|)
                    optstr += std::wstring(L"") + L"(?:" + *p + L"|)";
            }
			++p;
        }
        else if (*p == L'*' || *p == L'+' || *p == L'.' || *p == L'[' || *p == L']')
        {
//            throw exception(std::string("") + "現在は、メタ文字 " + p + " は利用できません。利用可能なメタ文字 () | .+ ?");
        }
        else
        {
            optstr += *p;
        }
    }

	this->LocalCaptureRuleNodeCount = 1;
	return AddRegexpImpl(id,optstr, stateHandle);
}

//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle)
{
	_USE_WINDOWS_ENCODING;
    std::wstring matchString;

	//正規表現をパースしながら回す.
	const wchar_t * p = str.c_str();
    const wchar_t * splitPos = p;
    rule*  currentRule = stateHandle;
	for(  ; *p ; ++p )
	{
		if (*p == L'(')
        {
            //閉じ括弧まで飛ばす. )
            int nest = 1;
            const wchar_t* n = p + 1;
            for( ; *n  ; ++n )
            {
                if (*n == L'(')
                {
                    ++nest ;
                }
                else if (*n == L')')
                {
                    --nest ;
                    if (nest <= 0)
                    {
                        break;
                    }
                }
            }

            //ネストする前の部分
            matchString = std::wstring(splitPos, 0 ,p - splitPos);


			//ネストしている部分を格納するルールを作る.
			rule*  nestRule;

			//キャプチャー？
			std::wstring captureNodeName;
            if (*(p+1) == L'?' && *(p+2) == L':')
            {
                p += 2;
            }
			else
			{
				captureNodeName = num2wstr(this->LocalCaptureRuleNodeCount++) + L":" + num2wstr(this->GlobalRuleNodeCount) ;
			}

			//文字列だけを入れると OR扱いになってしまうので、ネストを格納するネストを作らないとダメ。
			rule* firstNestRule = new rule(this->GlobalRuleNodeCount ++);
			firstNestRule->AddWord(_W2A(matchString.c_str()));

			//お目当ての()ネストを格納する部分を作る
			if (captureNodeName.length() <= 0)
			{
				nestRule = new rule(this->GlobalRuleNodeCount ++);
			}
			else
			{
				nestRule = new rule( _W2A(captureNodeName.c_str()) );
			}
			firstNestRule->AddNestRule(nestRule);
			currentRule->AddNestRule(firstNestRule);

			//かっこの後にも構文が連続する場合、そのツリーを作成する.
            if (*n == L'\0'  || *(n+1) == L'\0' || *(n+1) == L'|')
            {//閉じかっこで構文がとまる場合はそこで終端
				//nop
            }
            else
            {//さらに構文が続いている場合
            	rule * afterRule = new rule( this->GlobalRuleNodeCount++);
            	nestRule->SetCombineRule(afterRule);

				currentRule = afterRule;
            }

            //ネストしているルールを再帰して実行.
			matchString = std::wstring(p+1 , 0 , (int) (n - p - 1) );
			auto r = this->AddRegexpImpl(id,matchString, nestRule);
			if(!r)
			{
				return xreturn::errnoError(r);
			}

            p = n ;
            splitPos = n + 1;  //+1は最後の ) を飛ばす. iは forの ++i で i == splitPos となる。(わかりにくい)
        }
        else if (*p == L'|') 
        {
        	matchString = std::wstring(splitPos,0 , (int) (p - splitPos));
			if (matchString.length() >= 1)
            {
            	currentRule->AddWord(_W2A(matchString.c_str()));
			}
			//空分岐 (A|) のような場合、空ノードを入れる.
			if (matchString.length() <= 0 || *(p+1) == L'\0' )
			{
				currentRule->AddWord("");
			}

			splitPos = p + 1;
            currentRule = stateHandle;
        }
        else if (*p == L'.' && *(p+1) == L'+') 
        {
			currentRule->AddWord("＊");

			p += 1;
			splitPos = p + 1;
        }
    }

    //最後の残り
	matchString = std::wstring(splitPos , 0 , (int) (p - splitPos) );
    if ( matchString.length() >= 1 &&str.length() >= 1 && *(p-1) != L')')
    {
        currentRule->AddWord(_W2A(matchString.c_str()) );
    }
	return true;
}
