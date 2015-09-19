<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {

        if (empty($_POST["symbol"]))
        {
            apologize("You must enter a symbol to sell");
        }
        else if (empty($_POST["shares"]))
        {
            apologize("You must enter some shares to sell");
        }
        
        $sale = query("SELECT * FROM shares WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        if (empty($sale)) {
            apologize ("You don't own any shares of " . $_POST["symbol"]);
        }
        else if (!preg_match("/^\d+$/", $_POST["shares"]) && $sale[0]["shares"] > $_POST["shares"]) {
            apologize ("Please enter an integer number of shares to sell.");
            }
        
        else {
            if ($sale[0]["shares"] <= $_POST["shares"]) {
                query("DELETE FROM shares WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
            } else {
                query("UPDATE shares SET shares = shares - ? WHERE id = ? AND symbol = ?", $_POST["shares"], $_SESSION["id"], $_POST["symbol"]);
            }
            query("UPDATE users SET cash = cash + ? WHERE id = ?", $_POST["shares"]*lookup($_POST["symbol"])["price"], $_SESSION["id"]);
            
            query("INSERT INTO log (id, action, shares, symbol, price, date) VALUES(?, \"SELL\", ?, ?, ?, ?)", $_SESSION["id"], $_POST["shares"], strtoupper($_POST["symbol"]), lookup($_POST["symbol"])["price"], date('Y-m-d H:i:s'));
            
                header("Status: 302 Moved Temporarily");
                header("Location: ./index.php?saleAmt=" . min($_POST["shares"],$sale[0]["shares"]) . "&saleSymbol=" . $_POST["symbol"]);
                exit;
        }
        
    }
    else
    {
        // else render sell form
        render("sell_form.php", ["title" => "Sell Stock"]);
    }
?>
