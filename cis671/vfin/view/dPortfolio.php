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
                width: 70px;
                height: 32px;
                padding: 2px;
                font: 12px sans-serif;
                background: lightsteelblue;
                border: 0px;
                border-radius: 8px;
                pointer-events: none;
            }
            .legend rect {
              fill:white;
              stroke:black;
              opacity:0.8;
            }
        </style>
        <div id="graph" class="aGraph"></div>
        <input type="checkbox" id="cb1" /> Show Data Points
        <script>
            var div = d3.select("body").append("div")
                .attr("class", "tooltip")
                .style("opacity", 0);

            var parseTime = d3.timeParse("%d-%b-%y");
            var formatTime = d3.timeFormat("%e %b");
            var formatCurrency = d3.format("$,.2f");

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

                //console.log(data);

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

                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("id", "intrest_line")
                    .attr("stroke", "steelblue")
                    .attr("fill", "none")
                    .attr("stroke-width", 2)
                    .attr("d", line_total(data));
                // console.log(data);

                graph.selectAll("dots")
                    .data(data)
                    .enter().append("circle")
                        .attr("fill", "steelblue")
                        .attr("class", "circle1")
                        .attr("r", 3)
                        .attr("cx", function(d) { return x(d.date); })
                        .attr("cy", function(d) { return y(d.balance); })
                    .on("mouseover", function(d) {
                        div.transition()
                            .duration(200)
                            .style("opacity", .9);
                        div.html(formatTime(d.date) + "<br/>" + formatCurrency(d.balance))
                            .style("left", (d3.event.pageX) + "px")
                            .style("top", (d3.event.pageY - 28) + "px");
                        })
                            .on("mouseout", function(d) {
                        div.transition()
                            .duration(500)
                            .style("opacity", 0);
                        });

                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("id", "deposits_line")
                    .attr("stroke", "#71D161")
                    .attr("fill", "none")
                    .attr("stroke-width", 1)
                    .attr("d", line_deposit(data));


                graph.selectAll("dots")
                    .data(data)
                    .enter()
                    .filter(function(d) {
                        if(d.type === "deposit") {
                            return d;
                        }
                    })
                    .append("circle")
                        .attr("fill", "#71D161")
                        .attr("class", "circle1")
                        .attr("r", 3)
                        .attr("cx", function(d) { return x(d.date); })
                        .attr("cy", function(d) { return y(d.dBalance); })
                    .on("mouseover", function(d) {
                        div.transition()
                            .duration(200)
                            .style("opacity", .9);
                        div.html(formatTime(d.date) + "<br/>" + formatCurrency(d.dBalance))
                            .style("left", (d3.event.pageX) + "px")
                            .style("top", (d3.event.pageY - 28) + "px");
                        })
                            .on("mouseout", function(d) {
                        div.transition()
                            .duration(500)
                            .style("opacity", 0);
                        });

                legend = graph.append("g")
                    .attr("class", "legend")
                    .attr("x", 65)
                    .attr("y", 25)
                    .attr("height", 100)
                    .attr("width", 200);
                
                legend.append("rect")
                    .attr("x", 65)
                    .attr("y", 25)
                    .attr("width", 200)
                    .attr("height", 100);

                legend.append("circle")
                    .attr("class", "circle2")
                    .attr("cx", 85)
                    .attr("cy", 45)
                    .attr("r", 10)
                    .attr("stroke", "black")
                    .attr("fill", "#71D161")
                    .on("click", function() {
                        var active = deposits_line.active ? false : true;
                        var newOpacity = active ? 0 : 1;
                        d3.select("#deposits_line").style("opacity", newOpacity);
                        deposits_line.active = active;
                    });

                legend.append("text")
                    .attr("x", 110)
                    .attr("y", 50)
                    .text("Contributions");

                legend.append("circle")
                    .attr("class", "circle2")
                    .attr("cx", 85)
                    .attr("cy", 85)
                    .attr("r", 10)
                    .attr("stroke", "black")
                    .attr("fill", "steelblue")
                    .on("click", function() {
                        var active = intrest_line.active ? false : true;
                        var newOpacity = active ? 0 : 1;
                        d3.select("#intrest_line").style("opacity", newOpacity);
                        intrest_line.active = active;
                    });

                legend.append("text")
                    .attr("x", 110)
                    .attr("y", 90)
                    .text("Account Value");

                d3.select("#cb1")
                    .attr("x", 65);
                d3.select("#cb1").on("change",update);
                update();

                function update() {
                    if(d3.select("#cb1").property("checked")){
                        d3.selectAll(".circle1").attr("visibility", "visible")
                    } else {
                        d3.selectAll(".circle1").attr("visibility", "hidden");
                    }
                }
            });
        </script>
    </div>
</div>