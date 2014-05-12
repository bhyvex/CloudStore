<?php
error_reporting(E_ALL & ~ E_NOTICE);

define("HEADER_SIZE" , 20);
	class CHeader
	{
	    private $m_cmd;
	    private $m_length;
		private $m_error;
		private $m_para1; 
		private $m_para2;
		
	    function __construct($cmd = 0 , $length = 0, $error = 0, $para1 = 0, $para2 = 0)
		{
			$this->m_cmd = $cmd;
			$this->m_length = $length;
			$this->m_error = $error;
			$this->m_para1 = $para1;
			$this->m_para2 = $para2;
		}
		
	    function unpackage($head_string)
		{
			$this->m_header_string = $head_string;
			$binary_array = unpack("Icmd/Ilength/Ierror/Ipara1/Ipara2", $head_string);
			
			$this->m_cmd = $binary_array["cmd"];
			$this->m_length = $binary_array["length"];
			$this->m_error = $binary_array["error"];
			$this->m_para1 = $binary_array["para1"];
			$this->m_para2 = $binary_array["para2"];
		}

        function package()
        {
            $package_string = pack("I5", $this->m_cmd, $this->m_length, $this->m_error, $this->m_para1, $this->m_para2);
			//echo "string:".strlen($package_string)"<br/>";

            return $package_string;
        }

	    function cmd()
		{
			return $this->m_cmd;	
		}

        function set_cmd($cmd)
        {
            $this->m_cmd = $cmd;
        }
		
	    function length()
		{
			return $this->m_length;
		}

        function set_length($len)
        {
            $this->m_length = $len;
        }

	    function error()
	    {
		    return $this->m_error;	
	    }

        function set_error($error)
        {
            $this->m_error = $error;
        }
	
	    function para1()
	    {
		    return $this->m_para1;
        }

        function set_para1($para1)
        {
            $this->m_para1 = $para1;
        }
	
	    function para2()
	    {
		    return $this->m_para2;
	    }	

        function set_para2($para2)
        {
            $this->m_para2 = $para2;
        }
}
?>
