<?php
        if (!empty($lastTransaction)) {
            print($lastTransaction);
        }
?>
    
<div>
    <table class="table table-striped">

    <tr>
    <th>Stock</th>
    <th>Shares</th>
    <th>Price</th>
    <th>Total</th>
    </tr>
    <tr>
    <td>Cash</td>
    <td></td>
    <td></td>
    <td><?php echo $cash[0]["cash"] ?></td>
    </tr>
    <?php

        foreach ($positions as $position)
        {
            print("<tr>");
            print("<td>" . $position["symbol"] . "</td>");
            print("<td>" . number_format ($position["shares"],4) . "</td>");
            print("<td>" . number_format ($position["price"],4) . "</td>");
            print("<td>" . number_format ($position["shares"]*$position["price"],4) . "</td>");
            print("</tr>");
        }

    ?>
</table>

</div>
