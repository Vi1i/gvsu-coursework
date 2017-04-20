
<?php
//require_once("view/dummy.php");
	class IndexController{
		function IndexAction(){
			include_once("view/home.php");
		}

		function testAction(){
			include_once("view/test.php");
		}

		function sCreditAction(){
			include_once("view/sCredit.php");
		}

		function dCreditAction(){
			include_once("view/dCredit.php");
		}

		function dPortfolioAction(){
			include_once("view/dPortfolio.php");
		}
	}