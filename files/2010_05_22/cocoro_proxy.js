
   //オレの嫁を三人から選べって言うのか。
   //全部にきまってるんだろぉぉぉぉぉぉおお。
   var r = Math.random();
   var url;
   if (r <= 0.33)
   {//ココロ.
       url = "http://www.cocolog-nifty.com/cocolo/clock/js/cocolo_clock.js";
   }
   else if (r <= 0.66)
   {//むーちゃん.
       url = "http://www.cocolog-nifty.com/cocolo/clock/js/mu_clock.js";
   }
   else
   {//ココス.
       url = "http://www.cocolog-nifty.com/cocolo/clock/js/cocos_clock.js";
   }
   //動的に変更
   var script = document.createElement('script');
   script.src = url;
   document.body.appendChild(script);
