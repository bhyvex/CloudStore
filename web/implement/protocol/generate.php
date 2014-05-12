#!/usr/local/php/bin/php -f

<?php
    require_once('./parser/pb_parser.php');
    $parser = new PBParser();
//    $parser->parse('./test.proto');
    $parser->parse('./cstore.protocol.proto');

    echo "parser success ! \n";
?>
