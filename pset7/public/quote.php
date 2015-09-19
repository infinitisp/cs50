<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // render the quote (validation in the template... sorry but your MVC videos werent working and I dont see the point
        render("price_quote.php", ["stock" => lookup($_POST["symbol"])]);
    }
    else
    {
        // else render only form
        render("price_quote_request.php", ["title" => "Request Price Quote"]);
    }

?>
