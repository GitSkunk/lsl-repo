
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.


// defines for Ratany fine Engineerings Device Interface Protocol

#ifndef _RFEDIP
#define _RFEDIP

#define RFEDIP_sVERSION                           "RFEDIP-1.2"  // protocol version
#ifndef RFEDIP_sSUFFICIENT_VERSION
#define RFEDIP_sSUFFICIENT_VERSION                "RFEDIP"  // default to any version as sufficient version
#endif
#define RFEDIP_sSEP                               "|"  // separator used in protocol messages

#define RFEDIP_CHANNEL                            -20131224  // default channel used for protocol messages


// used to figure out whether a message is a RfE-dip message or not
//
#define RFEDIP_iMINMSGLEN                         6


// protocol control messages
//
// control messages must always be sent on the default channel
//
#define RFEDIP_protIDENTIFY_QUERY                 "identify"  // identification query, must be responded to with RFEDIP_protIDENTIFY
#define RFEDIP_protIDENTIFY                       "identity"  // answer to identification queries
#define RFEDIP_protEND                            "msg_end!"  // indicate end of data transfer; if a particular channel was opened, this channel can be closed
#define RFEDIP_protOPEN                           "openchan"  // open a particular channel for further communication, mandatorily has a channel number as parameter

// useful messages
//
// These messages are included for their usefulness.  They are not
// control messages so that they do not need to be sent on the default
// channel.
//
// an "OK" message that can be sent to indicate that data was
// received, that a requested operation was performed, etc.
//
#define RFEDIP_protOK                             "rfedipOK"


#define RFEDIP_idxSENDER                          0
#define RFEDIP_idxSENDER_UNIQ                     1
#define RFEDIP_idxRCPT                            2
#define RFEDIP_idxRCPT_UNIQ                       3
#define RFEDIP_idxPROTVERSION                     4
#define RFEDIP_idxTOKEN1                          5
#define RFEDIP_idxPARAM1                          6
#define RFEDIP_idxPARAM2                          7


// provide a default string to use as the uniq identifier of the sender
//
// llGetScriptName() appears to be a reasonable default since multiple
// scripts in the same prim cannot have the same name.  Therefore, a
// combination of the UUID of the prim the script is in and the name
// of the script itself makes for a sufficient identificator of the
// device (script).
//
#ifndef RFEDIP_sTHIS_UNIQ
#define RFEDIP_sTHIS_UNIQ                       llGetScriptName()
#endif


// get UUID of sender from list _l --- sender is other device
//
#define RFEDIP_ToSENDER(_l)                                           llList2Key(_l, RFEDIP_idxSENDER)

// get Uniq of sender from list _l --- sender is other device
#define RFEDIP_ToSENDER_UNIQ(_l)                                      llList2Key(_l, RFEDIP_idxSENDER_UNIQ)

// get UUID of recipient from list _l --- recipient is this device
//
#define RFEDIP_ToRCPT(_l)                                             llList2Key(_l, RFEDIP_idxRCPT)

// get Uniq of recipient from list _l --- recipient is this device
#define RFEDIP_ToRCPT_UNIQ(_l)                                        llList2Key(_l, RFEDIP_idxRCPT_UNIQ)

// get string containing the version of the protocol from list _l
//
#define RFEDIP_ToPROTVERSION(_l)                                      llList2String(_l, RFEDIP_idxPROTVERSION)

// get the first token from list _l
//
#define RFEDIP_ToFirstTOKEN(_l)                                       llList2String(_l, RFEDIP_idxTOKEN1)

// get the first parameter
//
#define RFEDIP_ToFirstPARAM(_l)                                       llList2String(_l, RFEDIP_idxPARAM1)

// get the second parameter
//
#define RFEDIP_ToSecondPARAM(_l)                                     llList2String(_l, RFEDIP_idxPARAM2)



// convert a protocol payload into a protocol message --- _sndr is this device
//
#define RFEDIP_ToRESPONSE(_sndr, _rcpt, _rcptUniq, ...)               llDumpList2String([_sndr, RFEDIP_sTHIS_UNIQ, _rcpt, _rcptUniq, RFEDIP_sVERSION, __VA_ARGS__], RFEDIP_sSEP)

// open a particular channel for communication --- _sndr is this device
//
#define REFDIP_OPEN(_rcpt, _rcptUniq, _sndr, _nchan)                  llRegionSayTo(_rcpt, RFEDIP_CHANNEL, RFEDIP_ToRESPONSE(_sndr, _rcpt, _rcptUniq, RFEDIP_protOPEN, _nchan))

// indicate end of communication on channel _c --- _sndr is this device
//
#define RFEDIP_END(_rcpt, _rcptUniq, _sndr, _nchan)                   llRegionSayTo(_rcpt, RFEDIP_CHANNEL, RFEDIP_ToRESPONSE(_sndr, _rcpt, _rcptUniq, RFEDIP_protEND, _nchan))

// ask all rfedip compliant devices to identify themselves --- sender is this device
//
#define RFEDIP_IDQUERY                                                llRegionSay(RFEDIP_CHANNEL, RFEDIP_protIDENTIFY_QUERY + RFEDIP_sSEP + RFEDIP_sTHIS_UNIQ)

// respond with an rfedip message --- _sndr is this device
//
#define RFEDIP_RESPOND(_rcpt, _rcptUniq, _sndr, _chan, ...)           llRegionSayTo(_rcpt, _chan, RFEDIP_ToRESPONSE(_sndr, _rcpt, _rcptUniq, __VA_ARGS__))

// send an OK message, _sndr is this device, _nchan is the channel to
// send the message on
//
// this is NOT a control message
//
#define RFEDIP_OK(_rcpt, _rcptUniq, _sndr, _nchan)                    llRegionSayTo(_rcpt, _nchan, RFEDIP_ToRESPONSE(_sndr, _rcpt, _rcptUniq, RFEDIP_protOK))


// macros to check version number
//
// Version numbers are composed of a leading string "RFEDIP-",
// followed by an integer representing the major version number and an
// integer representing the minor version number.  The two integers
// are seperated by a dot (".").
//
// This means that version "RFEDIP-1.115" is a higher version number
// than "RFEDIP-1.5".
//
// These two macros evaluate to the major and minor version numbers
// when provided with a protocol message that was converted to list
// _l.
//
#define RFEDIP_iVersionMajor(_l)                                      llList2Integer(llParseString2List(llList2String(llParseString2List(RFEDIP_ToPROTVERSION(_l), ["-"], []), 1), ["."], []), 0)
#define RFEDIP_iVersionMinor(_l)                                      llList2Integer(llParseString2List(llList2String(llParseString2List(RFEDIP_ToPROTVERSION(_l), ["-"], []), 1), ["."], []), 1)


#endif  // _RFEDIP
