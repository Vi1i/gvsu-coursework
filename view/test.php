<?php
$PageTitle = "vFin->Test";

function customPageHeader(){?>
  <!--Arbitrary HTML Tags-->
<?php }
include_once("header.php");
?>

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
        </style>
        <div id="graph" class="aGraph"></div>

        <script>
            /* implementation heavily influenced by http://bl.ocks.org/1166403 */
            function showPopover (d) {
                $(this).popover({
                    title: d.name,
                    placement: 'auto top',
                    container: 'body',
                    trigger: 'manual',
                    html : true,
                    content: function() { 
                        return "Quarter: " + d.label + "<br/>Rounds: " + d3.format(",")(d.value ? d.value: d.y1 - d.y0); }
                });
                $(this).popover('show')
            }
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

                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("clip-path", "url(#clip)")
                    .attr("stroke", "steelblue")
                    .attr("fill", "none")
                    .attr("d", line_total(data));

                var path = graph.append("svg:path")
                    .attr("class","path")
                    .attr("clip-path", "url(#clip)")
                    .attr("stroke", "steelblue")
                    .attr("fill", "none")
                    .attr("d", line_deposit(data))
                    .on("mouseover", function (d) { showPopover.call(this, d); })
                    .on("mouseout",  function (d) { removePopovers(); });
            });
        </script>
    </div>
</div>