#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "mkdfa.h"
extern "C"{
	extern int mkfa_main( int argc, const char *argv[] );
	extern int dfa_minimize_main( int argc, const char *argv[] );
}

bool mkdfa::conv(const std::string& gramprefix)
{
	std::string usrtmpdir = "./";
	std::string tmpdir = "./";

	int make_dict = 1;
	int make_term = 1;

	std::string gramfile = gramprefix + ".grammar";
	std::string vocafile = gramprefix + ".voca";
	std::string dfafile  = gramprefix + ".dfa";
	std::string dictfile = gramprefix + ".dict";
	std::string termfile = gramprefix + ".term";
	std::string tmpprefix = "_temp_gramprefix";
	std::string tmpvocafile = gramprefix + "_r.voca";
	std::string rgramfile = gramprefix + "_r.grammar";
	
	//grammer の並びを逆順に並び替えます
	{
		std::ifstream GRAM( gramfile );
		std::ofstream RGRAM( rgramfile );
		std::string line;
		while( std::getline(GRAM,line) )
		{
			boost::algorithm::trim(line);			
			if ( line.size() <= 1 || line[0] == '#')
			{
				continue;
			}
			std::list<std::string> lr;
			boost::algorithm::split(lr,line,std::bind2nd(std::equal_to<char>(), ':'));
			if (lr.size() < 2)
			{
				continue;
			}
			//lr[1]にアクセスしたい
			auto lr_left = lr.begin();
			auto lr_right = lr.begin(); lr_right ++ ;

			std::list<std::string> node;
			boost::algorithm::split(node,boost::algorithm::trim_copy(*lr_right),std::bind2nd(std::equal_to<char>(), ' '));

			std::reverse(node.begin(),node.end());

			RGRAM << *lr_left << ": " << boost::algorithm::join(node," ") << std::endl;
		}
	}

	//termid を付与していきます
	{
		std::ifstream VOCA( vocafile );
		std::ofstream TMPVOCA( tmpvocafile );
		std::ofstream GTERM( termfile );
		int n1 = 0;
		int n2 = 0;
		int termid = 0;
		std::string line;
		while( std::getline(VOCA,line) )
		{
			boost::algorithm::trim(line);			
			if ( line.size() <= 1 || line[0] == '#' || line[0] != '%')
			{
				continue;
			}
			std::string ll =line.c_str() + 1;
			boost::algorithm::trim(ll);

			TMPVOCA << "#" << ll << std::endl;
			GTERM << termid << "\t" << ll << std::endl;
			termid++;
		}
	}
	
	//dfaファイルを生成します。
	//こいつメモリーリークしまくっているのと、static変数を山盛り使っているので、
	//あとで全面的にリライトします。
	//   # minimize DFA after generation
	std::string _tmp_dfafile = dfafile + ".tmp";
	std::string _tmp_tmpprefix = tmpprefix + ".h";
	{
		const char *args[] =
		{
			"mkfa_main"
			,"-e1"
			,"-fg"
			,rgramfile.c_str()
			,"-fv"
			,tmpvocafile.c_str()
			,"-fo"
			,_tmp_dfafile.c_str()
			,"-fh"
			,_tmp_tmpprefix.c_str()
		};
		mkfa_main(10,args);
	}

	//dfa を最適化するらしい。
	//これsample grammer だと動作していないように思える。
	//ただ、ルーチンにはあるのでつけとくわー
	//こいつ julius のlibsentにべっとり依存しているので結構うざい。
	{
		const char *args[] =
		{
			"dfa_minimize_main"
			,_tmp_dfafile.c_str()
			,"-o"
			,dfafile.c_str()
		};
		dfa_minimize_main(4,args);
	}
	
	//dict ファイルを生成します。
	{
		std::ifstream VOCA( vocafile );
		std::ofstream DICT( dictfile );
	
		int nowid = -1;
		std::string line;
		while( std::getline(VOCA,line) )
		{
			boost::algorithm::trim(line);			
			if ( line.size() <= 1 || line[0] == '#')
			{
				continue;
			}
			if ( line[0] == '%' )
			{
				nowid++;
				continue;
			}
		
			std::list<std::string> node;
			boost::algorithm::split(node,line,std::bind2nd(std::equal_to<char>(), ' '));
		
			assert( node.size()>=2 );
			std::string name = *(node.begin());
			node.pop_front();

			DICT << nowid << "\t["	<< name << "]\t" 
				<< boost::algorithm::join(node , " ") << std::endl;
		}
	}
	
	//結局、julius が参照しているのは、 dfa と dict だけで後は不要です。
	return true;
}
