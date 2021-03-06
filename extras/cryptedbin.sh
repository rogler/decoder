#! /bin/sh
#
# create an export file with a CRYPTEDBINFILE section
#
# MUST BE EXECUTED ON A FRITZ!OS DEVICE
#
# remove the character device for an existing dvb.cfg file
#
rm /var/flash/dvb.cfg 2>/dev/null
#
# create our own file
#
cat >/var/flash/dvb.cfg <<'EOT'
/*
 * /var/flash/dvb.cfg
 * Sun May 14 20:57:35 2017
 */

meta { encoding = "utf-8"; }

version_dvb {
        revision = "$Revision: 1.0 $";
        creatversion = "1.00.00";
}


dvbcfg {
        nit_pid = 16;
        network_id = 61444;
        network_name = "Kabel Deutschland";
        bouquets = 32;
        tv_services = 101;
        radio_services = 67;
        other_services = 0;
        channels {
                sid = 53019;
                tsid = 10008;
                frequency = 442000000;
                lcn_nordig = 0;
                lcn_eacem = 0;
                flags = 32;
                url = 239.2.1.1;
                provider = "";
                name = "53019";
                type = 0;
                ca_mode = 0;
                pid_pmt = 220;
                pid_pcr = 0;
                pid_cnt = 0;
       }
}


// EOF
EOT
#
# declare TV functions active
#
export CONFIG_LINEARTV=y
#
# export the data with or without password
#
# ATTENTION:
#
# It looks like AVM disabled (accident or not, that's the question) the ability
# to export data without a password, since 2FA was added to the firmware.
# Exporting a file without password is not functioning on any of my 06.83 devices.
#
# We strip off all lines around the encrypted content.
#
tr069fwupdate configexport $1 | sed -n -e "/\*\*\*\* CRYPTEDBINFILE/,/\*\*\*\* END OF FILE/p" | sed -e "1d;\$d"
#
# data should be visible on STDOUT now
#
