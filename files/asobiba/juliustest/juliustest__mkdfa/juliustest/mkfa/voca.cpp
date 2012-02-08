
char *gettoken( char *line, char *dst );
BODY *appendTerm( BODY *list, char *name );
BODY *entryTerm( char *name, BODY *body, int listLen );
void freeTerm( void );

class voca
{
	std::list<CLASS*> ClassList;
	int SW_Quiet;
	int SW_Verbose;
	int NoNewLine;

	voca() : SW_Quiet(0),SW_Verbose(0),NoNewLine(0)
	{
	}
	virtual ~voca()
	{
		auto it = ClassList.begin();
		for(;it != ClassList.end() ; ++it )
		{
			delete * t;
		}
	}
	
	void setVoca( std::ifstream& VocaFile )
	{
	    char token1[ SYMBOL_LEN ];
	    char token2[ SYMBOL_LEN ];

	    int virgin = 1;
	    int bodynum = 0;
	    BODY *bodyList = NULL;

	    FILE *fp;
	    if( (fp = fopen( VocaFile, "r" )) == NULL ){
			errMes( "Can't open vocabulary file\"%s\"", VocaFile );
	    }

		char identifier[ SYMBOL_LEN ] = "";
	    if( !this->SW_Quiet ){
			newLineAdjust();
			fputs( "Now parsing vocabulary file\n", stderr );
	    }

	    while( 1 ){
			static char line[ 1000 ];
			char *ptr = line;
			if( fgets( line, 1000, fp ) == NULL ){
				entryTerm( identifier, bodyList, bodynum );
				break;
			}
			if( line[ 0 ] == '\0' ) continue;
			if( line[ 0 ] == '#' ){
				if( (ptr = gettoken( ptr, token1 )) == NULL ) continue;
				if( !virgin ){
					entryTerm( identifier, bodyList, bodynum );
					bodyList = NULL;
					bodynum = 0;
				} else {
					virgin = 0;
				}
				strcpy( identifier, token1 + 1 );
				continue;
			} else {
				ptr = gettoken( ptr, token1 );
				if( ptr == NULL ) continue;
				ptr = gettoken( ptr, token2 );
				if( ptr == NULL ){
					bodyList = appendTerm( bodyList, token1 );
				} else {
					bodyList = appendTerm( bodyList, token2 );
				}
				bodynum++;
			}
	    }
	}

	static char *gettoken( char *line, char *dst )
	{
	    char *ptr = dst;
	    char ch;

	    do{
			ch = *line++;
			if( ch == '\0' ) return( NULL );
		
	    } while( ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' );
	    while( 1 ){
			*ptr++ = ch;
			ch = *line++;
			if( ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ){
				*ptr = '\0';
				return( line );
			}
			if( ch == '\0' ){
				*ptr = '\0';
				return( line - 1 );
			}
	    }
	}

	BODY *appendTerm( BODY *list, char *name )
	{
/*
	    BODY *newTerm;

	    if( (newTerm = malloc( sizeof(BODY) )) == NULL ){
			errMes( "Can't alloc term list buffer" );
	    }
	    strcpy( newTerm->name, name );
	    newTerm->abort = 0;
	    newTerm->next = list;
	    return( newTerm );
*/
		
	}

BODY *entryTerm( char *name, BODY *body, int listLen )
{
    CLASS *classa;
    static int InputNo = 0;
/*    BODYLIST *bodyList; */

    if( getClass( name ) != NULL ){
		errMes( "Class redefined \"%s\"", name );
    }
    if( (classa = malloc( sizeof(CLASS) )) == NULL ){
		errMes( "Can't alloc memory for Class Finite Automaton." );
    }
    classa->no = InputNo++;
    strcpy( classa->name, name );
    classa->branch = -listLen;
    classa->usedFA = 0;
    classa->used=0;
    classa->tmp = 0;
		classa->next = NULL;
    if( ClassListTail == NULL ){
		ClassList = classa;
    } else {
		ClassListTail->next = classa;
    }
    ClassListTail = classa;
/*
    if( (bodyList = malloc( sizeof(BODYLIST) )) == NULL ){
	errMes( "Can't alloc nonterminal list buffer" );
    }
    bodyList->body = body;
    bodyList->next = NULL;
*/
    /* for test
    nt = class->nt[ 0 ];
    fprintf( stderr, "class: %s\n", class->name );
    while( 1 ){
	if( *nt->name == '\0' ) break;
	fprintf( stderr, "%s\n", nt->name );
	nt++;
    }*/

    return( NULL );
}
};
