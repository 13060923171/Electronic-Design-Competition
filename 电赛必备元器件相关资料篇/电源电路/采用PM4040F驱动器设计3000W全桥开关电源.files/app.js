var imgUrl=new Array();
var imgLink=new Array();
var sHTML=new Array();
var adNum=0;
imgUrl[1]="http://165v.com/165v/edit/UploadFile/200711010488238.jpg";
imgLink[1]="#";
sHTML[1]="���200W�Լ����ӱ�ѹ����Ӧ���������Դ�ͳ�������͵��ӵƹ��������������������﹫���ĵ�����Ʒ����Ʒ�������������ա������ɹ�������������������֧���»�Ϊ���ṩ����ɹ�������ʵ����Ʒ��";
imgUrl[2]="http://165v.com/165v/edit/UploadFile/2006311211418841.jpg";
imgLink[2]="#";
sHTML[2]="����ʽ���ص�Դ����ģ��Ӧ���ڴ��ʰ��ſ��ص�Դ�ϣ�ʹ���һ�����ŵ�Դ�Ե�ʮ�ּ򵥣�PM2020G������30A��IGBT��PM2060G������120A��IGBT�ܣ����ſ��ص�Դ������������ѹ17V������ѹ15V��";
imgUrl[3]="http://165v.com/165v/edit/UploadFile/200611520528423.jpg";
imgLink[3]="#";
sHTML[3]="ȫ�ſ��ص�Դר��ȫ������ģ�飬�ͺ�PM4020G������30A��IGBT��PM4060G������120A��IGBT�ܣ����һ�����ʵ�Դ��·ͬ��ʮ�ּ򵥣���������ģ�����18V���������ԡ�";
imgUrl[4]="http://165v.com/165v/edit/UploadFile/200611520344381.jpg";
imgLink[4]="#";
sHTML[4]="��Ƶ400Hz ��HIDȫ������ģ�����������ŵ�Ƶ�Դ�ļ򵥷�������HIDȫ������ģ���֧������HID�ƽ��ܵ������������ٷ�����������������������֧�ֻ�Ϊ���ṩ����ĳ�Ʒ���������ο���";
imgUrl[5]="http://165v.com/165v/edit/UploadFile/2005123214050744.jpg";
imgLink[5]="#";
sHTML[5]="���ӱ�ѹ������������Ĳ�Ʒ��������Ӵ�Ĺ�Ƶ��о��ѹ������÷�����������ӱ�ѹ�������ֱ����Ϊ�����ʹ��ʮ�ַ��㡣�������﹫���ĵ�����Ʒ����Ʒ���������ɹ���Ϊ���ṩ����֧�֡�";

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