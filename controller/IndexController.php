
<?php
//require_once("view/dummy.php");
	class IndexController{
		function IndexAction(){
			include_once("view/home.php");
		}

		function testAction(){
			include_once("view/test.php");
		}
	}