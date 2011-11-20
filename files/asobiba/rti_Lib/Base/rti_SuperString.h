class TSuperString
{
public:
	//指定された文字列を検索.
	static int FindString( const char * inBase , int inBaseLen  ,
					       const char * inChallenge , int inChallengeLen ,
						   bool inFindStartPos=false);
	//次の行へ.
	static int NextLine( const char * inBase , int inBaseLen  ,
										 const char * inRetCode="\r\n" );
	//指定された文字列を検索.
	static int FindString( const char * inBase	,
				 const char * inChallenge	,	bool inFindStartPos=false);
	//次の行へ.
	static int NextLine( const char * inBase , const char * inRetCode="\r\n" );
};

