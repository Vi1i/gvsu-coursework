(function (){
	var VIZ = {};

	VIZ.lineChart = function (data) {

	}

	VIZ.onResize = function () {
		var aspect = 1000 / 500, chart = $("#graph");
		var targetWidth = chart.parent().width();
		chart.attr("width", targetWidth);
		chart.attr("height", targetWidth / aspect);
	}
}())