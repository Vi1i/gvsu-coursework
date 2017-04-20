<?php
$PageTitle = "vFin->Test";

function customPageHeader(){?>
  <!--Arbitrary HTML Tags-->
<?php }
include_once("header.php");
?>


<script src="https://d3js.org/d3-selection-multi.v0.4.min.js"></script>
<div class="container">
    <div class="jumbotron">
        <style>
            .axis {
                shape-rendering: crispEdges;
            }

            .x.axis line {
                stroke: lightgrey;
            }

            .x.axis .minor {
                stroke-opacity: .5;
            }

            .x.axis path {
                display: none;
            }

            .x.axis path {
                display: none;
            }
            div.tooltip {
                position: absolute;
                text-align: center;
                width: 60px;
                height: 28px;
                padding: 2px;
                font: 12px sans-serif;
                background: lightsteelblue;
                border: 0px;
                border-radius: 8px;
                pointer-events: none;
            }
        </style>
        <div id="graph" class="aGraph"></div>

        <script>
            var div = d3.select("body").append("div")
                .attr("class", "tooltip")
                .style("opacity", 0);

            var parseTime = d3.timeParse("%d-%b-%y");
            var formatTime = d3.timeFormat("%e %B");

            var dep_balance = 0;
            var prev_balance = 0;
            var data = d3.csv("/data/portfolio-mod.csv", function(d) {
                d.balance = +d.balance;
                // This little block turns the data into d3 readable
                if(d.type === "deposit") {
                    dep_balance += d.balance - prev_balance;
                }
                d.dBalance = dep_balance;
                d.date = new Date(Date.parse(d.date));
                d.net = +d.net;

                prev_balance = d.balance;
                return d;
            }, function(error, data) {
                if (error) throw error;

                console.log(data);

                // define dimensions of graph
                var margins = {top: 20, left: 20, bottom: 30, right: 50};
                var width = 1000 - margins.left - margins.right;
                var height = 500 - margins.top - margins.bottom;




                var x = d3.scaleTime().rangeRound([0, width]);
                var y = d3.scaleLinear().rangeRound([height, 0]);

                var line_total = d3.line()
                    .x(function(d) { return x(d.date); })
                    .y(function(d) { return y(d.balance); });

                var line_deposit = d3.line()
                    .x(function(d) { return x(d.date); })
                    .y(function(d) { return y(d.dBalance); });


                x.domain(d3.extent(data, function(d) { return d.date; }));
                y.domain(d3.extent(data, function(d) { return d.balance; }));

                // Add an SVG element with the desired dimensions and margin.
                var graph = d3.select("#graph").append("svg:svg")
                    .attr("width", width + margins.left + margins.right)
                    .attr("height", height + margins.top + margins.bottom)
                    .append("svg:g")
                    .attr("transform", "translate(" + (margins.right + 10) + "," + margins.top + ")");

                // create x Axis
                var xAxis = d3.axisBottom().scale(x).tickSize(-height);
                // Add the x-axis.
                graph.append("svg:g")
                    .attr("class", "x axis")
                    .attr("transform", "translate(0," + height + ")")
                    .call(xAxis)
                    .append("text")
                        .attr("fill", "#000")
                        //.attr("transform", "rotate(-90)")
                        .attr("y", 20)
                        .attr("x", (width + 10) / 2)
                        .attr("dy", "0.71em")
                        .attr("text-anchor", "end")
                        .text("Balance Date");

                // create left yAxis
                var yAxisLeft = d3.axisLeft().scale(y).tickFormat(d3.format("$,.0f"));
                // Add the y-axis to the left
                graph.append("svg:g")
                    .attr("class", "y axis")
                    //.attr("transform", "translate(-25,0)")
                    .call(yAxisLeft)
                    .append("text")
                        .attr("fill", "#000")
                        .attr("transform", "rotate(-90)")
                        .attr("y", -60)
                        .attr("x", -height / 2)
                        .attr("dy", "0.71em")
                        .attr("text-anchor", "end")
                        .text("Balance");

                const focus = graph.append('g')
                    .attr('class', 'focus')
                    .style('display', 'none');

                // var path = graph.append("svg:path1")
                //     .attr("class","path1")
                //     .attr("stroke", "steelblue")
                //     .attr("fill", "none")
                //     .attr("d", line_total(data));

                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("stroke", "steelblue")
                    .attr("fill", "none")
                    .attr("d", line_deposit(data));

                graph.selectAll("dots")
                    .data(data)
                    .enter().append("circle")
                        .attr("r", 1)
                        .attr("cx", function(d) { return x(d.date); })
                        .attr("cy", function(d) { return y(d.dBalance); })
                    .on("mouseover", function(d) {
                        div.transition()
                            .duration(200)
                            .style("opacity", .9);
                        div.html(formatTime(d.date) + "<br/>" + d.dBalance)
                            .style("left", (d3.event.pageX) + "px")
                            .style("top", (d3.event.pageY - 28) + "px");
                        })
                            .on("mouseout", function(d) {
                        div.transition()
                            .duration(500)
                            .style("opacity", 0);
                        });
            });
        </script>
    </div>
</div>