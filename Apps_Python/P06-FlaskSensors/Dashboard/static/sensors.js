
$(document).ready(function () {

var data = [];
var dataset1;

var Tempdata = [];
var dataset2;

var totalPoints = 10;
var updateInterval = 500;			
var now = new Date().getTime();

function GetAmbientLight()
{
	$.getJSON( '/GetLight', function( json ) 
	{
	  console.log( "JSON Data: " + json.CH1 + " " + json.CH2 + " " + json.LuxVAL );  
	 
		 var y ;
		 var temp;
		 
		 if(data.length >= totalPoints)
		 {
			data.shift();
			y = json.LuxVAL;
		 }else{
			y = json.LuxVAL;
		 }
		 
		 temp = [now += updateInterval, y];
		 data.push(temp);
	
	 });
}	


var options1 = {
	series: {
		lines: {
			show: true,
			lineWidth: 2.2,
			fill: true
		}
	},
	xaxis: {
		mode: "time",
		tickSize: [1, "second"],
		tickFormatter: function (v, axis) 
		{
			var date = new Date(v);
			if (date.getSeconds() % 1 == 0) {
				var hours = date.getHours() < 10 ? "0" + date.getHours() : date.getHours();
				var minutes = date.getMinutes() < 10 ? "0" + date.getMinutes() : date.getMinutes();
				var seconds = date.getSeconds() < 10 ? "0" + date.getSeconds() : date.getSeconds();

				return hours + ":" + minutes + ":" + seconds;
			} else {
				return "";
			}
		},
		axisLabel: "Time",
		axisLabelUseCanvas: true,
		axisLabelFontSizePixels: 12,
		axisLabelFontFamily: 'Verdana, Arial',
		axisLabelPadding: 10
	},
	yaxis: {
		min: 0,
		max: 100,        
		tickSize: 10,
		tickFormatter: function (v, axis) {
			if (v % 10 == 0) {
				return v + "%";
			} else {
				return "";
			}
		},
		axisLabel: "Ambient Light",
		axisLabelUseCanvas: true,
		axisLabelFontSizePixels: 12,
		axisLabelFontFamily: 'Verdana, Arial',
		axisLabelPadding: 6
	},
	legend: {        
		labelBoxBorderColor: "#fff"
	},
	grid: {                
		backgroundColor: "#000000",
		tickColor: "#008040" 
	}
};


	GetAmbientLight();
	
	dataset1 = [{ label: "Light", data: data, color: "#00FF00" }];

	$.plot($("#flot-placeholder1"), dataset1, options1);
	

	function update() {
		GetAmbientLight();		
		$.plot($("#flot-placeholder1"), dataset1, options1)
		if (realtime === "on")
		{
		setTimeout(update, updateInterval);
		}
	}			
	
		//INITIALIZE REALTIME DATA FETCHING
	if (realtime === "on") 
	{
	  update();
	}
	//REALTIME TOGGLE
	$("#realtime .btn").click(function () {
	  if ($(this).data("toggle") === "on") {
		realtime = "on";
	  }
	  else {
		realtime = "off";
	  }
	  update();
	});
	
	
	 /*
   * INITIALIZE BUTTON TOGGLE
   * ------------------------
   */
  $('.btn-group[data-toggle="btn-toggle"]').each(function () {
    var group = $(this);
    $(this).find(".btn").on('click', function (e) {
      group.find(".btn.active").removeClass("active");
      $(this).addClass("active");
      e.preventDefault();
    });

  });
 
});