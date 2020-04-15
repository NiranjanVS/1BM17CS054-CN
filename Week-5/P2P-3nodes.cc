// Default Network Topology
//
//       10.1.1.0          10.1.2.0
// n0 -------------- n1 -------------- n2
//    point-to-point    point-to-point

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (3);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices,devices2;
  devices = pointToPoint.Install (nodes.Get(0),nodes.Get(1));// node 0 is of index 0 and node 1 is of index 1
  devices2 = pointToPoint.Install (nodes.Get(1),nodes.Get(2));// node  1 is of index 0 and node 2 is of index 1 ,these are used further

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  UdpEchoServerHelper echoServer (9);
  UdpEchoServerHelper echoServer2 (10);// dose'nt work on same port so use the differrent port

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  ApplicationContainer serverApps2 = echoServer2.Install (nodes.Get (1));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  //address(0) means ... in pointToPoint.Install devices are taken as 0 and 1, int he first line node 0 is 0 and node 1 is 1in the second line node 1 is 0 and node 2 is 1
  UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (0), 10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (2));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
