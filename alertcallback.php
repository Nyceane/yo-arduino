<?php
/**
 * Yo PHP
 * A PHP class for the Yo API from dev.justyo.co
 * 
 * @Author Maarten Paauw
 */
 
require_once("class.yo.php");

$token = "621c8499-45ee-4ed1-a33f-c7c01106626d";
$link = "https://www.google.com/maps/place/Portico+Restaurant/@37.7899008,-122.3978871,19z/data=!4m2!3m1!1s0x0000000000000000:0x3cadb0a535edcb95";
$username = "CARSENSE";

$yo = new yo($token);

// Send a Yo with a link to a specific subscriber.
$yo->user($username);

printf($yo);
?>