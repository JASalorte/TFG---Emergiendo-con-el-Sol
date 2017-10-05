<?php

$var1 = $_POST['var1'];
$var2 = $_POST['var2'];


require './mysqlData.php';

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$var3 = $var1 + 86400;
//$sql = "SELECT node, state FROM reserve_table WHERE plane = '$var2' AND (UNIX_TIMESTAMP('$var1') BETWEEN startdate AND enddate)";

$sql = "SELECT * FROM test WHERE central = '$var2' AND (date >= $var1 AND date < $var3)";
    //echo $sql;
$result = $conn->query($sql);

$var1 = array();
$var2 = array();
$var3 = array();
$var4 = array();
$var5 = array();
$var6 = array();
$var7 = array();
$var8 = array();
$var9 = array();
$var10 = array();
$var11 = array();
$var12 = array();
$var13 = array();
$var14 = array();
$var15 = array();


if ($result->num_rows > 0) {
    // output data of each row
    while ($row = $result->fetch_assoc()) {
        $row["date"] = $row["date"] * 1000;
        array_push($var1, array($row["date"], $row["var1"]));
        array_push($var2, array($row["date"], $row["var2"]));
        array_push($var3, array($row["date"], $row["var3"]));
        array_push($var4, array($row["date"], $row["var4"]));
        array_push($var5, array($row["date"], $row["var5"]));
        array_push($var6, array($row["date"], $row["var6"]));
        array_push($var7, array($row["date"], $row["var7"]));
        array_push($var8, array($row["date"], $row["var8"]));
        array_push($var9, array($row["date"], $row["var9"]));
        array_push($var10, array($row["date"], $row["var10"]));
        array_push($var11, array($row["date"], $row["var11"]));
        array_push($var12, array($row["date"], $row["var12"]));
        array_push($var13, array($row["date"], $row["var13"]));
        array_push($var14, array($row["date"], $row["var14"]));
        array_push($var15, array($row["date"], $row["var15"]));
    }
}

$return = array("Tension AC" => array("label" => "Tension AC", "data" => $var1), 
    "Corriente AC" => array("label" => "Corriente AC", "data" => $var2),
    "Potencia activa" => array("label" => "Potencia activa", "data" => $var3),
    "Potencia aparente" => array("label" => "Potencia aparente", "data" => $var4),
    "Potencia reactiva" => array("label" => "Potencia reactiva", "data" => $var5),
    "Frecuencia" => array("label" => "Frecuencia", "data" => $var6),
    "Factor de potencia" => array("label" => "Factor de potencia", "data" => $var7),
    "Energia total" => array("label" => "Energia total", "data" => $var8),
    "Energia parcial" => array("label" => "Energia parcial", "data" => $var9),
    "Corriente DC" => array("label" => "Corriente DC", "data" => $var10),
    "Tension DC" => array("label" => "Tension DC", "data" => $var11),
    "Potencia DC" => array("label" => "Potencia DC", "data" => $var12),
    "Irradiancia" => array("label" => "Irradiancia", "data" => $var13),
    "Temp modulo" => array("label" => "Temp modulo", "data" => $var14),
    "Temp ambiente" => array("label" => "Temp ambiente", "data" => $var15)
    );



echo json_encode($return);

$conn->close();

?>