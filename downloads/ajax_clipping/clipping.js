
// Initialise ajax links when dom is ready
$(document).ready(function(){
updateLinks();
});

/*
 * Transform all anchors within clipping areas and clipping anchors
 */
function updateLinks(url) {

   // Enable this to see which links will be transformed to ajax clipping links	
   // $("#clipping_area a").css("border","3px solid red");
   // $("clipping_link").css("border","3px solid red");

    $(".clipping_area a").unbind('click');
    $(".clipping_area a").bind('click',function(event){
      event.preventDefault();
	var node = this;
        var areaId = null;
	while(node != null) {
	   node = node.parentNode;
	   if(node.getAttribute("class")=='clipping_area') {
		areaId = node.getAttribute("id");
		break;
	   }	
   	   alert(node);
	}
        loadPage(this.href,areaId);
    });

    $(".clipping_link").unbind('click');
    $(".clipping_link").bind('click',function(event){
      event.preventDefault();
      loadPage(this.href,this.target);
    });

    // handle images that 
    $("img").error(function(){

    var pageURL = url;
    var imageURL  = this.src;
    
    var imagePath = imageURL.replace(/(\/[^\/]*$)/,"");
    var imageFileName = imageURL.replace(/^.*\//,"");
    var pagePath =  pageURL.replace(/(\/[^\/]*$)/,"");
    var diffPath = pagePath.replace(imagePath,"");
    var newImageURL =  imagePath + diffPath +"/"+ imageFileName;
    // set the corrected url for the image
    this.src = newImageURL;

 });

}

/*
 * Load the given url into the clipping area with the given areaId
 */
function loadPage(url,areaId) {
    $.get(url,
       function(data) {
         setPage(data,areaId);
  	 updateLinks(url);
    });
}

/*
 * Parse the given string and create a dom object
 */
function parseXML( xml ) {
   if( window.ActiveXObject && window.GetObject ) {
        var dom = new ActiveXObject( 'Microsoft.XMLDOM' );
        dom.loadXML( xml );
        return dom;
    }
    if( window.DOMParser )
       return new DOMParser().parseFromString( xml, 'text/xml' );
    throw new Error( 'No XML parser available' );
} 

/*
 * Parse the given html and locate body tag. Insert html into 
 * the given clipping area by using the areaId.
 */
function setPage(html,areaId) {
   var dom = parseXML(html);
   var $dom = $(dom); 
   var strXML = dom.getElementsByTagName('body').item(0).innerHTML;
   $("#"+areaId).html(strXML);
}
