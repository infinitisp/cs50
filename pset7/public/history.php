<?php

    // configuration
    require("../includes/config.php"); 

    //get history info
    $rows = query("SELECT * FROM log WHERE id = ?", $_SESSION["id"]);
    if (empty($rows)) 
        apologize("Sorry- you have no history!");
    render("history_table.php", ["logs" => $rows, "title" => "History"]);

?>
