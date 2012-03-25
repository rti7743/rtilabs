-- c-like lua
function add(x,y) {
    return x + y
}

//階乗
function factorial_while(n) {
    //whileで階乗してみる
    sum = 1
    i = 1
    while(i <= n){
       sum = sum * i
       i = i + 1
    }
    return sum
}

//階乗
function factorial_for(n) {
    //forで階乗してみる
    //すまないが、 lua のforを 完全にc-likeにしきれていないんだ
    sum = 1
    for (i = 1 , n , 1 ) { //or for (i = 1 , n ) {
       sum = sum * i;
    }
    return sum
}

//山川
function yamaorkawa(str) {
    table = {} //もちろん大丈夫
    if (str == "yama") {
         return "kawa"
    }
    else if (str == "kawa") {
         return "yama"
    }
    else {
         return "nazo";
    }
}

//山川
function yamaorkawa_luastyle(str)
    if str == "yama" then
         return "kawa"
    elseif str == "kawa" then
         return "yama"
    else
         return "nazo";
    end
end

//山川
function yamaorkawa_sjis(str) {
    if (str == "山") {
         return "川"
    }
    else if (str == "川") {
         return "山"
    }
    return "イミフwww"
}

//ダメ文字列で出力
print( l_cecho("癒ソニーの表") );
