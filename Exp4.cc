#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  //Take Logs
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);


  //Take n number of Nodes
  NodeContainer nodes;
  nodes.Create (2);

  //Choose your Technology to Communicate
  PointToPointHelper pointToPoint;  //P2P Channel
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  //Install Technology on Computers
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  //Asking to Follow Rules
  InternetStackHelper stack;
  stack.Install (nodes);

  //Assign IP Address to Communicate
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");  //("IP Address", "Subnet Mask")
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  //Create a X type of server on port X
  UdpEchoServerHelper echoServer (19);

  //Install server on a node then start and stop the server
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //Create a X type of Client and set its attributes
  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 19);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //Install the server then Start and Stop it
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
