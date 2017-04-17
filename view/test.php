<?php
$PageTitle = "vFin->Test";

function customPageHeader(){?>
  <!--Arbitrary HTML Tags-->
<?php }
include_once("header.php");
?>

<!-- <link rel="stylesheet" href="view/css/styles.css"> -->

 <link rel="stylesheet" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.7.2/themes/ui-lightness/jquery-ui.css" type="text/css" media="all" />
 <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.23/jquery-ui.min.js"></script>

<style>
    /* tell the SVG path to be a thin blue line without any area fill */
/*    path {
        stroke: steelblue;
        stroke-width: 1;
        fill: none;
    }*/

    .axis {
        shape-rendering: crispEdges;
    }

    .x.axis line {
        stroke: lightgrey;
    }

    .x.axis .minor {
        stroke-opacity: .5;
    }

    .x.axis path domain {
        display: none;
    }

    .x.axis path {
        display: none;
    }

    .y.axis line, .y.axis path {
        fill: none;
        stroke: #000;
    }
</style>

<div class="container">
    <div class="jumbotron">
        <div id="graph" class="aGraph"></div>
        <div id="slider-range" style="width: 80%px; margin-left:10%; margin-right:10%"></div>

        <script>
            /* implementation heavily influenced by http://bl.ocks.org/1166403 */
            // define dimensions of graph
            var m = [20, 20, 30, 50]; // margins
            var w = 1000 - m[1] - m[3]; // width
            var h = 500 - m[0] - m[2]; // height
            var parseTime = d3.timeParse("%m/%d/%Y");

            var x = d3.scaleTime().rangeRound([0, w]);
            var y = d3.scaleLinear().rangeRound([h, 0]);
            var line = d3.line()
                .x(function(d) { return x(d["Trans Date"]); })
                .y(function(d) { return y(d.Total); });

            var total = 0;
            var data = d3.csv("/data/creditCard-mod.csv", function(d) {
                // This little block turns the data into d3 readable
                d["Trans Date"] = parseTime(d["Trans Date"]);
                d["Post Date"] = parseTime(d["Post Date"]);
                d.Amount = +d.Amount;
                total += d.Amount;
                d.Total = total;
                return d;
            }, function(error, data) {
                if (error) throw error;

                x.domain(d3.extent(data, function(d) { return d["Trans Date"]; }));
                y.domain(d3.extent(data, function(d) { return d.Total; }));

                // Add an SVG element with the desired dimensions and margin.
                var graph = d3.select("#graph").append("svg:svg")
                    .attr("width", w + m[1] + m[3])
                    .attr("height", h + m[0] + m[2])
                    .append("svg:g")
                    .attr("transform", "translate(" + m[3] + "," + m[0] + ")");

                // create yAxis
                var xAxis = d3.axisBottom().scale(x).tickSize(-h);//.tickSubdivide(3);
                // Add the x-axis.
                graph.append("svg:g")
                    .attr("class", "x axis")
                    .attr("transform", "translate(0," + h + ")")
                    .call(xAxis);

                //This is the the 0 axis line
                graph.append("svg:g")
                    .attr('transform', 'translate(' + 0 + ', ' + y(0) + ')')
                    .attr("stroke", "black")
                    .call(d3.axisBottom(x).tickSizeInner(0).tickFormat("").tickSizeOuter(0))
                    .select(".domain")

                // create left yAxis
                var yAxisLeft = d3.axisLeft().scale(y).tickFormat(d3.format("$,.0f"));//.ticks(4)//.orient("left");
                // Add the y-axis to the left
                graph.append("svg:g")
                    .attr("class", "y axis")
                    //.attr("transform", "translate(-25,0)")
                    .call(yAxisLeft);

                var clip = graph.append("defs").append("svg:clipPath")
                    .attr("id", "clip")
                    .append("svg:rect")
                    .attr("id", "clip-rect")
                    .attr("x", "0")
                    .attr("y", "0")
                    .attr("width", w)
                    .attr("height", h);


                // Add the line by appending an svg:path element with the data line we created above
                // do this AFTER the axes above so that the line is above the tick-lines
                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("clip-path", "url(#clip)")
                    .attr("stroke", "steelblue")
                    .attr("fill", "none")
                    .attr("d", line(data));

                function zoom(begin, end) {
                    x.domain([begin, end - 1]);

                    var t = graph.transition().duration(0);

                    var size = end - begin;
                    var step = size / 10;
                    var ticks = [];
                    for (var i = 0; i <= 10; i++) {
                        ticks.push(Math.floor(begin + step * i));
                    }

                    xAxis.tickValues(ticks);

                    t.select(".x.axis").call(xAxis);
                    t.select('.path').attr("d", line(data));
                    return data;
                }

                var minDate = Date.parse(d3.min(d3.extent(data, function(d) { return d["Trans Date"]; })))/1000;
                var maxDate = Date.parse(d3.max(d3.extent(data, function(d) { return d["Trans Date"]; })))/1000;
                var dates = [];
                data.forEach(function(d) {
                    dates.push(Date.parse(d["Trans Date"])/1000);
                })

                console.log(minDate);
                console.log(maxDate);
                console.log(dates);

                $(function() {
                    $("#slider-range").slider({
                        range: true,
                        min: minDate,
                        max: maxDate,
                        values: [minDate, maxDate],
                        slide: function( event, ui ) {
                            console.log(ui);
                            var begin = d3.min([ui.values[0], data.length]);
                            var end = d3.max([ui.values[1], 0]);
                            console.log("begin:", begin, "end:", end);

                            zoom(begin, end);
                        }
                    });
                });
            });
        </script>
    </div>
</div>