<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {

        if (empty($_POST["symbol"]))
        {
            apologize("You must enter a symbol to buy");
        }
        else if (!preg_match("/^\d+$/", $_POST["shares"]))
        {
            apologize("You must enter some integer number of shares to buy");
        }
        else if (empty(lookup($_POST["symbol"])["price"])) {
            apologize("That stock does not exist.");
        }
        
        $totalPrice = $_POST["shares"]*lookup($_POST["symbol"])["price"];
        $cash = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
        
        if ( $cash[0]["cash"] < $totalPrice) {
            apologize ("You can't afford that.");
        }
        else {
            $existingStock = query("SELECT * FROM shares WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
            
            if (!empty($existingStock)) {
                query("UPDATE shares SET shares = shares + ? WHERE id = ? AND symbol = ?", $_POST["shares"], $_SESSION["id"], $_POST["symbol"]);
            } else {
                query("INSERT INTO shares (id, symbol, shares) VALUES(?, ?, ?)", $_SESSION["id"], strtoupper($_POST["symbol"]), $_POST["shares"]);
            }
            query("UPDATE users SET cash = cash - ? WHERE id = ?", $totalPrice, $_SESSION["id"]);
            
            query("INSERT INTO log (id, action, shares, symbol, price, date) VALUES(?, \"BUY\", ?, ?, ?, ?)", $_SESSION["id"], $_POST["shares"], strtoupper($_POST["symbol"]), lookup($_POST["symbol"])["price"], date('Y-m-d H:i:s'));
            
            
                header("Status: 302 Moved Temporarily");
                header("Location: ./index.php?buyAmt=" . max($_POST["shares"],$sale[0]["shares"]) . "&buySymbol=" . $_POST["symbol"]);
                exit;
        }
        
    }
    else
    {
        // else render buy form
        render("buy_form.php", ["title" => "Buy Stock"]);
    }
?>
