<?php
$PageTitle = "vFin->Test";

function customPageHeader(){?>
  <!--Arbitrary HTML Tags-->
<?php }
include_once("header.php");
?>

<div class="container">
    <div class="jumbotron">
        <div id="graph" class="aGraph"></div>

        <script>
            /* implementation heavily influenced by http://bl.ocks.org/1166403 */
            var data = d3.csv("/data/portfolio-mod.csv", function(d) {
                // This little block turns the data into d3 readable
                d.balance = +d.balance;
                d.date = new Date(Date.parse(d.date));
                d.net = +d.net;

                return d;
            }, function(error, data) {
                if (error) throw error;

                console.log(data);

                // define dimensions of graph
                var margins = {top: 20, left: 20, bottom: 30, right: 50};
                var width = 1000 - margins.left - margins.right;
                var height = 500 - margins.top - margins.bottom;
            });
        </script>
    </div>
</div>