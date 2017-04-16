<?php
$PageTitle = "vFin->Test";

function customPageHeader(){?>
  <!--Arbitrary HTML Tags-->
<?php }
include_once("header.php");
?>

<!-- <link rel="stylesheet" href="view/css/styles.css"> -->

<div class="container">
    <div class="jumbotron">
        <svg id="visulization" width="1000" height="500"
	  		viewBox="0 0 1000 500"
	  		preserveAspectRatio="xMidYMid meet">
		</svg>
        <script>
        var svg = d3.select("#visulization"),
            margin = {top: 20, right: 20, bottom: 30, left: 50},
            width = +svg.attr("width") - margin.left - margin.right,
            height = +svg.attr("height") - margin.top - margin.bottom,
            g = svg.append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");

        var parseTime = d3.timeParse("%m/%d/%Y");
        var x = d3.scaleTime().rangeRound([0, width]);
        var y = d3.scaleLinear().rangeRound([height, 0]);
        var line = d3.line()
            .x(function(d) { return x(d["Trans Date"]); })
            .y(function(d) { return y(d.Amount); });
        //console.log(csv);
        d3.csv("/data/creditCard-mod.csv", function(d) {
        	// This little block turns the data into d3 readable
        	d["Trans Date"] = parseTime(d["Trans Date"]);
        	d["Post Date"] = parseTime(d["Post Date"]);
        	d.Amount = +d.Amount;
        	return d;
        }, function(error, data) {
        	if (error) throw error;

        	x.domain(d3.extent(data, function(d) { return d["Trans Date"]; }));
        	y.domain(d3.extent(data, function(d) { return d.Amount; }));

        	g.append("g")
        	    .attr("transform", "translate(0," + height + ")")
        	    .call(d3.axisBottom(x))
        	  .select(".domain")
        	    .remove();

        	g.append("g")
        	    .call(d3.axisLeft(y))
        	  .append("text")
        	    .attr("fill", "#000")
        	    .attr("transform", "rotate(-90)")
        	    .attr("y", 6)
        	    .attr("dy", "0.71em")
        	    .attr("text-anchor", "end")
        	    .text("Price ($)");

        	g.append("path")
        	    .datum(data)
        	    .attr("fill", "none")
        	    .attr("stroke", "steelblue")
        	    .attr("stroke-linejoin", "round")
        	    .attr("stroke-linecap", "round")
        	    .attr("stroke-width", 1.5)
        	    .attr("d", line);

        });
        </script>
    </div>
</div>