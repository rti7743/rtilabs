/*
c-like lua 
*/
#include <windows.h>
#include <assert.h>
#include "lua/lua.hpp"


//luaに登録する関数
int l_cecho(lua_State* L)
{
	//第1引数
	if (! lua_isstring(L,-1) )
	{
		return luaL_error(L,"文字列ぢゃなっぽ");
	}
	printf("c_echo 文字列: %s\r\n" , lua_tostring(L,-1) );

	//戻り値
	lua_pushstring(L,"'リソーの表'戻り値だにょー");

	//lua_setusertag で入れた値が取れるようにしてみた。
	//thisの引き回しとかだるいから、これにしなイカ？
	//
	//ふつーに書くと、セットして
	//lua_pushlightuserdata(L ,(void*) this);
	//lua_setfield(L,"_this");
	//
	//取り出し、めんどい
	//lua_getfield(L,LUA_REGISTRYINDEX,"_this");
	//myclass* p = (myclass*)lua_touserdata(L,-1);
	//lua_pop(L,1);
	//
	assert( (unsigned long)lua_getusertag(L) == (unsigned long)0xff00ff00);


	return 1;             //戻り値の数を指定 1つ
}

int main()
{
    // Luaを開く
    lua_State* L = luaL_newstate();
	//マルチバイト(SJISとか)が使えるように修正するフラグを作ってみた。
	lua_setmultibyte(L,1);
	//ついでに this 引き回しに使えるフラグを追加してみた。
	lua_setusertag(L,(void*)0xff00ff00);


	//こちらの関数登録.
	lua_register(L, "l_cecho", (lua_CFunction)l_cecho);

    // Lua標準関数を使う
    luaL_openlibs(L);

    // Luaファイルを読み込む
	
    if(luaL_loadfile(L, "sample.lua") || lua_pcall(L, 0, 0, 0)) {
        perror(lua_tostring(L, -1));
    }

	//add関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "add");
		//第1引数x
		lua_pushnumber(L, 10);
		//第2引数y
		lua_pushstring(L,"20");

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 2, 1, 0) != 0) {
				printf("add(10,20)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("add(10,20)結果 : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	//factorial_while関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "factorial_while");
		//第1引数n
		lua_pushnumber(L, 10);

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("factorial_while(10)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("factorial_while(10)結果 : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	//factorial_for関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "factorial_for");
		//第1引数n
		lua_pushnumber(L, 10);

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("factorial_for(10)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isnumber(L, -1) ) {
				printf("factorial_for(10)結果 : %d\n", lua_tointeger(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	//yamaorkawa関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "yamaorkawa");
		//第1引数n
		lua_pushstring(L, "yama");

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa(yama)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa(yama)結果 : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	//yamaorkawa_luastyle関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "yamaorkawa_luastyle");
		//第1引数n
		lua_pushstring(L, "yama");

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa_luastyle(yama)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa_luastyle(yama)結果 : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	//yamaorkawa_sjis関数デモ
	{
		//add関数をスタックに積む
		lua_getglobal(L, "yamaorkawa_sjis");
		//第1引数n
		lua_pushstring(L, "yama");

		//add(x, y)を呼び出す 引数2個，戻り値1個
		if(lua_pcall(L, 1, 1, 0) != 0) {
				printf("yamaorkawa_sjis(yama)関数呼び出し失敗\n");
				printf("error : %s\n", lua_tostring(L, -1) );
				return 1;
		}
		if( lua_isstring(L, -1) ) {
				printf("yamaorkawa_sjis(yama)結果 : %s\n", lua_tostring(L, -1) );
				lua_pop(L,1); //戻り値をポップ
		}
	}
	

    // Luaを閉じる
    lua_close(L);

    printf("エンターキーを押すと終了しますりんこ\n");
	getchar();
    return 0;
}