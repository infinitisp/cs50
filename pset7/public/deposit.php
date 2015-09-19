<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (!preg_match("/^\d+$/", $_POST["amount"]))
            apologize("Must enter a positive integer amount.");
        query("UPDATE users SET cash = cash + ? WHERE id = ?", $_POST["amount"], $_SESSION["id"]);  
            
                header("Status: 302 Moved Temporarily");
                header("Location: ./index.php?depositAmt=" . $_POST["amount"]);
                exit;
    }
    else
    {
        // else render only form
        render("deposit_form.php", ["title" => "Deposit Cash"]);
    }

?>
