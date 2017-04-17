<?php
//echo "hello";
    // $username = "homedb"; 
    // $password = "user1234!";   
    // $host = "localhost";
    // $database="Finance";
    
    // $server = mysql_connect($host, $username, $password);
    // $connection = mysql_select_db($database, $server);

    // $myquery = "SELECT  `date`, `close` FROM  `data2`";
    // $query = mysql_query($myquery);

    // if ( ! $query ) {
    //     echo mysql_error();
    //     die;
    // }

    // $data = array();

    // for ($x = 0; $x < mysql_num_rows($query); $x++) {
    //     $data[] = mysql_fetch_assoc($query);
    // }

    // echo json_encode($data);     

    // mysql_close($server);

require_once('utils/Router.php');
require_once('controller/IndexController.php');

$router = new Router();
$router->route('/^\/test$/','IndexController::testAction');
$router->route('/^\/static$/','IndexController::StaticAction');
$router->route('/^\/$/','IndexController::IndexAction');
$router->execute($_SERVER['REQUEST_URI']);
