<?php

    // configuration
    require("../includes/config.php"); 

    //get portfolio info
    $rows = query("SELECT * FROM shares WHERE id = ?", $_SESSION["id"]);
    $cash = query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
    
    $lastTransaction = "";
    //see if any GET data; if so, pass it
    if (!empty($_GET["saleAmt"]) && !empty($_GET["saleSymbol"])) {
        $lastTransaction = "<div>Awesome. You sold " . $_GET["saleAmt"] . " shares of " . $_GET["saleSymbol"] . ".</div> <br />";
    }
    if (!empty($_GET["buyAmt"]) && !empty($_GET["buySymbol"])) {
        $lastTransaction = "<div>Awesome. You bought " . $_GET["buyAmt"] . " shares of " . $_GET["buySymbol"] . ".</div> <br />";
    }
    if (!empty($_GET["depositAmt"])) {
        $lastTransaction = "<div>Awesome. You deposited " . $_GET["depositAmt"] . "!</div> <br />";
    }
    
    // render portfolio
    $positions = [];
    foreach ($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $positions[] = [
                "name" => $stock["name"],
                "price" => $stock["price"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"]
            ];
        }
    }
    
    render("portfolio.php", ["lastTransaction" => $lastTransaction, "cash" => $cash, "positions" => $positions, "title" => "Portfolio"]);

?>
