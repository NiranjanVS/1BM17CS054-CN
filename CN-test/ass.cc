// Default Network Topology
//
//       10.1.1.0                            30.1.3.0
// m0 -------m1------- n1   n2   (S)n3   n4--------n5--------n6
//    point-to-point  |    |      |    |  point-to-point
//                    ===================
//                      LAN 20.1.2.0


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 2;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

  NodeContainer nodes;
  nodes.Create (3);

  NodeContainer nodes2;
  nodes2.Create (3);

  NodeContainer csmaNodes;
  csmaNodes.Add (nodes.Get (2));
  csmaNodes.Add (nodes2.Get (0));
  csmaNodes.Create (nCsma);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices,p2pDevices1;
  p2pDevices = pointToPoint.Install (nodes.Get(0),nodes.Get(1));
  p2pDevices1 = pointToPoint.Install (nodes.Get(1),nodes.Get(2));

  NetDeviceContainer p2pDevices2,p2pDevices3;
  p2pDevices2 = pointToPoint.Install (nodes2.Get(0),nodes2.Get(1));
  p2pDevices3 = pointToPoint.Install (nodes2.Get(1),nodes2.Get(2));

//   InternetStackHelper stack;
//   stack.Install (nodes.Get(0));
//   stack.Install (nodes.Get(1));

//   InternetStackHelper stack2;
//   stack.Install (nodes2.Get(1));
//   stack.Install (nodes2.Get(2)); 

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (nodes.Get (0));
  stack.Install (nodes.Get (1));
  stack.Install (csmaNodes);
  stack.Install  (nodes2.Get (1));
  stack.Install (nodes2.Get (2));

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices1);
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("20.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("30.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces2;
  p2pInterfaces2 = address.Assign (p2pDevices2);
   p2pInterfaces2 = address.Assign (p2pDevices3);
 

  UdpEchoServerHelper echoServer (9), echoServer2 (10);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  ApplicationContainer serverApps2 = echoServer2.Install (csmaNodes.Get (nCsma));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  UdpEchoClientHelper echoClient2 (csmaInterfaces.GetAddress (nCsma), 10);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  ApplicationContainer clientApps2 = echoClient2.Install (nodes2.Get (2));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  pointToPoint.EnablePcapAll ("second");
  csma.EnablePcap ("second", csmaDevices.Get (1), true);
  pointToPoint.EnablePcapAll ("second");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
/*At time 2s client sent 1024 bytes to 20.1.2.3 port 9
At time 2s client sent 1024 bytes to 20.1.2.3 port 10
At time 2.00749s server received 1024 bytes from 10.1.1.3 port 49153
At time 2.00749s server sent 1024 bytes to 10.1.1.3 port 49153
At time 2.01049s server received 1024 bytes from 30.1.3.4 port 49153
At time 2.01049s server sent 1024 bytes to 30.1.3.4 port 49153
At time 2.01798s client received 1024 bytes from 20.1.2.3 port 10
At time 2.02098s client received 1024 bytes from 20.1.2.3 port 9*/
