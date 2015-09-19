<div><h1>
    <?php 
    if (empty($stock)) {
    
    echo "Invalid stock symbol. Try Again.";
    }
    else{
    
    echo 'Stock price for<br />' . $stock["name"] . ':<br />' . number_format ( $stock["price"], 4) . '.';
    }
    ?>
    </h1>
</div>
<br />

    <?php require("price_quote_request.php"); ?>
