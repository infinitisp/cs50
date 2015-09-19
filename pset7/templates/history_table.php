<div>
    <table class="table table-striped">

    <tr>
    <th>Action</th>
    <th>Stock</th>
    <th>Shares</th>
    <th>Price</th>
    <th>Total</th>
    <th>Time</th>
    </tr>
    <?php

        foreach ($logs as $log)
        {
            print("<tr>");
            print("<td>" . $log["action"] . "</td>");
            print("<td>" . $log["symbol"] . "</td>");
            print("<td>" . number_format ($log["shares"],4) . "</td>");
            print("<td>" . number_format ($log["price"],4) . "</td>");
            print("<td>" . number_format ($log["shares"]*$log["price"],4) . "</td>");
            print("<td>" . $log["date"] . "</td>");
            print("</tr>");
        }

    ?>
</table>

</div>
