<?php

    // configuration
    require("../includes/config.php");

    // if form was submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
         // validate submission
        if (empty($_POST["username"]))
        {
            apologize("You must provide your username.");
        }
        else if (empty($_POST["password"]))
        {
            apologize("You must provide your password.");
        }
        else if ($_POST["password"] != $_POST["confirmation"])
        {
            apologize("Passwords did not match.");
        }

        // Insert user
        if (query("INSERT INTO users (username, hash, cash) VALUES(?, ?, 10000.00)", $_POST["username"], crypt($_POST["password"])) === false) 
        {
            apologize("Error: could not register new user.");
        }
        else //success
        {
            $rows = query("SELECT LAST_INSERT_ID() AS id");
            $id = $rows[0]["id"];
            
            // remember that user's now logged in by storing user's ID in session
            $_SESSION["id"] = $id;

            // redirect to portfolio
            redirect("/");
        }
        

    }
    else
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

?>
