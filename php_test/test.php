<?php

function get_stock_class_url($stock_class, &$url)
{
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, "http://tw.stock.yahoo.com/h/getclass.php");
  // 以字串傳回結果
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  // 支援header重導向(僅3次)
  curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
  curl_setopt($curl, CURLOPT_MAXREDIRS, 3);
  // 設定超過10秒就算失敗
  curl_setopt($curl, CURLOPT_TIMEOUT, 10);
  // search URL in <a>XXX</a>
  $result = preg_match('/<a' .
                       '(?:\s++(?!href)[A-Z][-:A-Z0-9]*+(?:\s*+=\s*+(?:"[^"]*+"|\'[^\']*+\'|[-.:\w]++))?+)*+' .
                       '\s++href\s*+=\s*+(?>"([^"]*+)"|\'([^\']*+)\'|([-.:\w]++))' .
                       '[^<>]*+>' .
                       $stock_class .
                       '<\/a>/ix',
                       curl_exec($curl),
                       $matches);
  curl_close($curl);
  if($result <= 0)
    return false;
    
  for($i = 1; $i < count($matches); $i++)
  {
    if(!isset($matches[$i]) || strlen($matches[$i]) <= 0)
      continue;
    
    $pos = strrpos($matches[$i], "?");
    if($pos === FALSE)
      continue;
      
    $url = 'http://tw.stock.yahoo.com' . str_replace(" ", "+", $matches[$i]);
    return true;
  }
  return false;
}


function get_stock_class_table($url)
{
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, $url);
  // 以字串傳回結果
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  // 支援header重導向(僅3次)
  curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
  curl_setopt($curl, CURLOPT_MAXREDIRS, 3);
  // 假裝從 http://tw.stock.yahoo.com/h/getclass.php click URL
  curl_setopt($curl, CURLOPT_REFERER, 'http://tw.stock.yahoo.com/h/getclass.php');
  // 設定超過10秒就算失敗
  curl_setopt($curl, CURLOPT_TIMEOUT, 10);
  // 取得 html 順便搜尋 <table>...</table>
  $match_result = preg_match_all('/<table[^>]*+>([^<]*+(?:(?!<\/?+table)<[^<]*+)*+)<\/table>/i', 
                           curl_exec($curl),
                           $table_matches,
                           PREG_SET_ORDER);
  curl_close($curl);
  if($match_result === FALSE || $match_result <= 0)
    return false;
  
  // 找出內含股票代號的 table
	echo $table_matches[0][1];
	echo "\n";
  for($i = 0; $i < count($table_matches); $i++)
  {
    #if(strpos($table_matches[$i][1], 'xAA\xD1\xB2\xBC\xA5\x4E\xB8\xB9') > 0)
    if(strpos($table_matches[$i][1], '股票代號') > 0)
      break;
  }
  
  if(count($table_matches) <= $i)
    return false;
  
  // search <tr>...</tr>
  $match_result = preg_match_all('/<tr[^>]*+>([^<]*+(?:(?!<\/?+tr)<[^<]*+)*+)<\/tr>/ix', 
                                 $table_matches[$i][1], 
                                 $tr_matches, 
                                 PREG_SET_ORDER);
  if($match_result === FALSE || $match_result <= 0)
    return false;
    
  // search <td>...</td> and print out
  for($i = 0; $i < count($tr_matches); $i++)
  {
    $match_result = preg_match_all('/<td[^>]*+>([^<]*+(?:(?!<\/?+td)<[^<]*+)*+)<\/td>/ix', 
                                   $tr_matches[$i][1], 
                                   $td_matches, 
                                   PREG_SET_ORDER);
    if($match_result === FALSE || $match_result <= 0)
      continue;

    // 移除 <td>...</td> 內的 HTML tag 與換行符號
    echo trim(str_replace("\n", "", preg_replace('/<[^>]++>/', '', $td_matches[1][1])));
    for($j = 2; $j < count($td_matches)-1; $j++)
    {
        echo ',' . trim(preg_replace("/[\n,]/", '', preg_replace('/<[^>]++>/', '', $td_matches[$j][1])));
    }      
    echo "\n";
  }
  
  return true;
}

// '\xA5\x62\xBE\xC9\xC5\xE9' = 半導體
if(get_stock_class_url('\xA5\x62\xBE\xC9\xC5\xE9', $url))
{
  if(!get_stock_class_table($url))
  {
    echo "get_stock_class_table() fail!\n";  
    die();  
  }
}
else
{
  echo "get_stock_class_url() fail!\n";  
}


?>
