<?php
/**
 * Yo PHP
 * A PHP class for the Yo API from dev.justyo.co
 * 
 * @Author Maarten Paauw
 */
 
require_once("class.yo.php");

$token = "c4e42f58-a108-43c0-bbb4-9248fbcbc30d";
$link = "https://www.google.com/maps/place/Portico+Restaurant/@37.7899008,-122.3978871,19z/data=!4m2!3m1!1s0x0000000000000000:0x3cadb0a535edcb95";
$username = "MITOMAN";

$yo = new yo($token);

// Send a Yo with a link to a specific subscriber.
$yo->user($username, $link);

printf($yo);
?>