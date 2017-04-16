<?php

/*
*
* As seen in http://upshots.org/php/php-seriously-simple-router
* 					 http://upshots.org/php/php-regexrouter
*
*/
require_once('controller/IndexController.php');
//require('controller/RegistrationController.php');
//require('controller/NotificationController.php');

class Router {

	private $routes = array();

	public function __construct() {}
	private function __clone() {}

	public function route($pattern, $callback) {
		if (isset($this->routes[$pattern])) {
			trigger_error("RegexRouter route already exist:	$pattern", E_USER_NOTICE);
			return;
		}
		$this->routes[$pattern] = $callback;
	}

	public function execute($uri) {
		foreach ($this->routes as $pattern => $callback) {
				if (preg_match($pattern, $uri, $params) === 1) {
					if (is_string($callback) && strpos( $callback, '::' ) ) {
						list( $controller, $method ) = explode( '::', $callback );
						$callback = array( new $controller, $method );
					}
					array_shift($params);
					return call_user_func_array($callback, array_values($params));
				}
		}
	}
}