var InitDemo = function () {
	console.log("Initialization Start");
	
	var canvas = document.getElementById("glCanvas");
	
	var glOptions = ["webgl", "experimental-webgl"];
	for (var i = 0; i < glOptions.length; i++) {
		var gl = canvas.getContext(glOptions[i]);
		if(gl) {
			console.log("GL Context Config: "+glOptions[i]);
			break;
		}
	}
	if(!gl) {
		alert("Current browser does not support WebGL");
	}
	canvas.width  = window.innerWidth;
	canvas.height = window.innerHeight;
	gl.viewport(0, 0, window.innerWidth, window.innerHeight);
	gl.clearColor(0.75, 0.85, 0.8, 1.0);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	console.log("Initialization Finished");
}