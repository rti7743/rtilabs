//
//このソースに含まれるプログラムは、 prototype.js からコピペして編集したものを含んでいます。
//prototype.js が MIT ライセンスなんで問題ないと思います。
//また、 jquery.js からのコピペも含まれています。
//jquery.jsも mit ライセンスなんでこれまた問題ないと思います。
//
//最終的には各自の判断で使ってください。
//作者(rti)の著作は気にしないで結構です。NYSLって感じで。 てきとーに使ってください。うひゃひゃ。
//


//スクリプトの削除
function _stripScripts(scriptsString) {
	var ScriptFragment = '<script[^>]*>([\\S\\s]*?)<\/script>';
	return scriptsString.replace(new RegExp(ScriptFragment, 'img'), '');
}

//スクリプトだけを抽出し、配列に格納
function _extractScripts(scriptsString) {
	var ScriptFragment = '<script[^>]*>([\\S\\s]*?)<\/script>';
	var matchAll = new RegExp(ScriptFragment, 'img');
	var matchOne = new RegExp(ScriptFragment, 'im');

	return jQuery.map(
		scriptsString.match(matchAll) || [] , 
		function(scriptTag) {
			return (scriptTag.match(matchOne) || ['', ''])[1];
		}
	);
}

//スクリプトを実行する.
function _evalScripts(scriptsString) {
	jQuery.map( _extractScripts(scriptsString) ,
		function(script) { jQuery.globalEval(script); return true; }
	);
}

//IE7?
function isIE7()
{
	return ( typeof document.documentElement.style.msInterpolationMode != 'undefined' );
}


//MAC
function isMac()
{
	var agent = navigator.userAgent.toLowerCase();

	return agent.indexOf("macintosh") != -1 || agent.indexOf("mac os") != -1;
}

//Chrome
function isChrome()
{
	var agent = navigator.userAgent.toLowerCase();

	return agent.indexOf("chrome") != -1 ;
}


//めんどうだから、 selector の部分とはばっさり削除! まさに外道
jQuery.fn.extend({
	super_load: function( url, params, callback ) {
		callback = callback || function(){};

		// Default to a GET request
		var type = "GET";

		// If the second parameter was provided
		if ( params )
			// If it's a function
			if ( jQuery.isFunction( params ) ) {
				// We assume that it's the callback
				callback = params;
				params = null;

			// Otherwise, build a param string
			} else {
				params = jQuery.param( params );
				type = "POST";
			}

		var self = this;

		// Request the remote document
		jQuery.ajax({
			url: url,
			type: type,
			dataType: "html",
			data: params,
			complete: function(res, status){
				// If successful, inject the HTML into all the matched elements
				if ( status == "success" || status == "notmodified" )
				{
					//IE7未満は javascript を書き込もうとするとエラーになるので、
					//prototype.jsのやり方で、javascriptだけを実行する実行してあげる必要がある.
					//Mac Safari も同様のバグがある。 windows safariは直っているのになんで?
					//Chrome! お前もか!
					if (	(jQuery.browser.msie && !isIE7()) 
						||  (jQuery.browser.safari && isMac() ) 
						||  ( isChrome() )
						)
					{
						//HTMLだけを書き込む
						self.html(_stripScripts(res.responseText));
						//javascriptを実行する
						_evalScripts(res.responseText);
					}
					else
					{
						//IE7以上または、FFやOperaはjQueryのいつものやり方。
						self.html(res.responseText);
					}
				}
				//エラーのときもコールバックしようぜ! コールバック
				self.each( callback, [res.responseText, status, res] );
			}
		});
		return this;
	}
});
