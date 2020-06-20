var imgUrl=new Array();
var imgLink=new Array();
var sHTML=new Array();
var adNum=0;
imgUrl[1]="http://165v.com/165v/edit/UploadFile/200711010488238.jpg";
imgLink[1]="#";
sHTML[1]="最简单200W自激电子变压器可应用于音响电源和充电器、和电子灯光领域，请留意我们在这里公布的电子作品及成品技术，供您仿照、助您成功。电子制作网在您的支持下会为您提供更多成功技术和实验作品。";
imgUrl[2]="http://165v.com/165v/edit/UploadFile/2006311211418841.jpg";
imgLink[2]="#";
sHTML[2]="半桥式开关电源驱动模块应用于大功率半桥开关电源上，使设计一个半桥电源显得十分简单，PM2020G可驱动30A的IGBT管PM2060G可驱动120A的IGBT管，半桥开关电源驱动器启动电压17V工作电压15V。";
imgUrl[3]="http://165v.com/165v/edit/UploadFile/200611520528423.jpg";
imgLink[3]="#";
sHTML[3]="全桥开关电源专用全桥驱动模块，型号PM4020G可驱动30A的IGBT管PM4060G可驱动120A的IGBT管，设计一个大功率电源电路同样十分简单！测试驱动模块采用18V可正常测试。";
imgUrl[4]="http://165v.com/165v/edit/UploadFile/200611520344381.jpg";
imgLink[4]="#";
sHTML[4]="低频400Hz 的HID全桥驱动模块是设计气体放电灯电源的简单方案，有HID全桥驱动模块的支持生产HID灯节能电子镇流器不再繁琐，电子制作网有了您的支持会为您提供更多的成品技术供您参考。";
imgUrl[5]="http://165v.com/165v/edit/UploadFile/2005123214050744.jpg";
imgLink[5]="#";
sHTML[5]="电子变压器是最简单最成熟的产品，是替代庞大的工频铁芯变压器的最好方案，这个电子变压器可输出直流作为充电器使用十分方便。留意这里公布的电子作品及成品技术助您成功！为您提供技术支持。";

var imgPre=new Array();
var j=0;
for (i=1;i<=5;i++) {
if( (imgUrl[i]!="") && (imgLink[i]!="") && (sHTML[i]!="") ) {
j++;
} else {
break;
}
}

function playTran(){
if (document.all)
imgInit.filters.revealTrans.play();
}
var key=0;
function nextAd(){
if(adNum<j)adNum++ ;
else adNum=1;

if( key==0 ){
key=1;
} else if (document.all){
imgInit.filters.revealTrans.Transition=6;
imgInit.filters.revealTrans.apply();
                   playTran();

}
document.images.imgInit.src=imgUrl[adNum];
idInit.innerHTML=sHTML[adNum];
theTimer=setTimeout("nextAd()", 15000);
}

function goUrl(){
jumpUrl=imgLink[adNum];
jumpTarget='_self';
if (jumpUrl != ''){
if (jumpTarget != '') 
window.open(jumpUrl,jumpTarget);
else
location.href=jumpUrl;
}
}