<?php
require_once('utils/Router.php');
require_once('controller/IndexController.php');

$router = new Router();
$router->route('/^\/test$/','IndexController::testAction');
$router->route('/^\/sCredit$/','IndexController::sCreditAction');
$router->route('/^\/dCredit$/','IndexController::dCreditAction');
$router->route('/^\/$/','IndexController::IndexAction');
$router->execute($_SERVER['REQUEST_URI']);
