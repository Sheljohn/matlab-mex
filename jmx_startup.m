function jmx_startup()

    here = fileparts(mfilename('fullpath'));
    dk.println('[JMX] Starting up from folder "%s".',here);
    dk.env.path_flag( 'JMX_ROOT', here );

end