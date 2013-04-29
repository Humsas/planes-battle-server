/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Header:       FGOutputFG.h
 Author:       Bertrand Coconnier
 Date started: 09/10/11

 ------------- Copyright (C) 2011 Bertrand Coconnier -------------

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place - Suite 330, Boston, MA  02111-1307, USA.

 Further information about the GNU Lesser General Public License can also be found on
 the world wide web at http://www.gnu.org.

HISTORY
--------------------------------------------------------------------------------
11/09/07   HDW   Added FlightGear Socket Interface
09/10/11   BC    Moved the FlightGear socket in a separate class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef FGOUTPUTFG_H
#define FGOUTPUTFG_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "FGOutputSocket.h"
#include "input_output/net_fdm.hxx"
#include "input_output/FGfdmSocket.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define ID_OUTPUTFG "$Id: FGOutputFG.h,v 1.1 2012/09/05 21:49:19 bcoconni Exp $"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace JSBSim {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Implements the output to a FlightGear socket.
 */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class FGOutputFG : public FGOutputSocket
{
public:
  /** Constructor that read the output directives from an XML file.
      @param fdmex a pointer to the parent executive object
      @param element XML Element that is pointing to the output directives
      @param idx ID of the output instance that is constructed
   */
  FGOutputFG(FGFDMExec* fdmex, Element* element, int idx);
  /** Constructor to which all the needed data are passed by parameters. When an
      instance is generated by this constructor, the methods SetOutputName(),
      SetProtocol() and SetPort() must be called afterwards to set the socket
      parameters. Otherwise the OS will likely complain that you try to open an
      invalid socket.
      @param fdmex a pointer to the parent executive object
      @param idx ID of the output instance that is constructed
      @param subSystems bitfield that describes the activated subsystems
      @param protocol network protocol for outputs directed to sockets
      @param port port to which the socket will be directed
      @param name IP address (e.g. 192.168.0.10, or machine_name.mycompany.com)
                  to which the output will be directed
      @param outRate output rate in Hz
      @param outputProperties list of properties that should be output
   */
  FGOutputFG(FGFDMExec* fdmex,  int idx, int subSystems, std::string protocol,
             std::string port, std::string name, double outRate,
             std::vector<FGPropertyManager *> & outputProperties);

  virtual void Print(void);

private:
  FGNetFDM fgSockBuf;
  void SocketDataFill(FGNetFDM* net);
};
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif

