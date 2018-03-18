package main

import (
	"fmt"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/golang/protobuf/proto"
	"github.com/nats-io/go-nats"
	"github.com/xackery/rebuildeq/go/eqproto"
)

var (
	nc       *nats.Conn
	err      error
	entities []*eqproto.Entity
)

func main() {

	if nc, err = nats.Connect(nats.DefaultURL); err != nil {
		log.Fatal(err)
	}
	defer nc.Close()

	zone := "ecommons"
	entities = testZoneCommandEntityList(zone, "entitylist", []string{"npc"})
	//entities.Entities = append(entities.Entities, testZoneCommandEntityList(zone, "entitylist", []string{"client"})...)
	fmt.Println(len(entities), "entities known")
	var attackEntityID int64
	for _, entity := range entities {
		if entity.Name == "Guard_Reskin000" {
			fmt.Println("Found ping as ID", entity.Id)
			attackEntityID = int64(entity.Id)
			break
		}
	}
	if attackEntityID == 0 {
		log.Fatal("Can't find guard to attack!")
	}
	//return
	//testSyncSubscriber()
	//go testAsyncSubscriber()
	//go testBroadcastMessage()
	//testAsyncSubscriber("EntityEvent")

	entityID := testZoneCommandEntity(zone, "spawn", []string{
		"146.17",
		"-112.51",
		"-52.01",
		"109.6",
		"GoSpawn",
	})
	if entityID == 0 {
		log.Fatal("failed to get entity ID!")
	}
	go testMoveToLoop(zone, entityID)
	go testAttack(zone, entityID, attackEntityID)
	go testAsyncEntityEventSubscriber(zone, entityID)
	//testZoneMessage("fieldofbone", "hello, world!")
	time.Sleep(1000 * time.Second)
}

func testAsyncSubscriber(channel string) {
	nc.Subscribe(channel, func(m *nats.Msg) {
		log.Printf("Received a message: %s\n", string(m.Data))
	})
	log.Println("Waiting on messages...")

	time.Sleep(500 * time.Second)
}

//testMoveToLoop causes an npc to go in a circle in pojustice
func testMoveToLoop(zone string, entityID int64) {
	params := []string{}
	positions := []string{
		"156.72 -136.71 -52.02 112.8",
		"116.18 -101.56 -51.56 228.8",
		"151.37 -102.54 -52.01 228.8",
	}
	command := "moveto"
	curPos := 0
	for {
		curPos++
		fmt.Println("Moving to position", curPos)
		if len(positions) < curPos+1 {
			fmt.Println("Resetting position")
			curPos = 0
		}

		params = []string{}
		params = append(params, fmt.Sprintf("%d", entityID))
		params = append(params, strings.Split(positions[curPos], " ")...)

		testZoneCommand(zone, command, params)
		time.Sleep(5 * time.Second)
	}
}

func testAttack(zone string, entityID int64, targetID int64) {

	time.Sleep(10 * time.Second)
	fmt.Println("10 seconds, Having", entityID, "attack", targetID)
	params := []string{
		fmt.Sprintf("%d", entityID),
		fmt.Sprintf("%d", entities[0].Id), //attack first element
		"1",
	}
	command := "attack"
	testZoneCommand(zone, command, params)
}

func testZoneCommandEntityList(zone string, command string, params []string) (entities []*eqproto.Entity) {

	msg := &eqproto.CommandMessage{
		Author:  "xackery",
		Command: command,
		Params:  params,
	}
	d, err := proto.Marshal(msg)
	if err != nil {
		log.Fatal(err)
	}
	reply, err := nc.Request(fmt.Sprintf("zone.%s.command_message", zone), d, 1*time.Second)
	if err != nil {
		log.Println("Failed to get request response:", err.Error())
		return
	}

	err = proto.Unmarshal(reply.Data, msg)
	if err != nil {
		fmt.Println("Failed to unmarshal", err.Error())
		return
	}
	rootEntities := &eqproto.Entities{}
	err = proto.Unmarshal([]byte(msg.Payload), rootEntities)
	if err != nil {
		fmt.Println("failed to unmarshal entities", err.Error(), msg)
		return
	}
	entities = rootEntities.Entities
	return
}

func testZoneCommandEntity(zone string, command string, params []string) (entityID int64) {

	msg := &eqproto.CommandMessage{
		Author:  "xackery",
		Command: command,
		Params:  params,
	}
	d, err := proto.Marshal(msg)
	if err != nil {
		log.Fatal(err)
	}
	reply, err := nc.Request(fmt.Sprintf("zone.%s.command_message", zone), d, 1*time.Second)
	if err != nil {
		log.Println("Failed to get request response:", err.Error())
		return
	}

	err = proto.Unmarshal(reply.Data, msg)
	if err != nil {
		fmt.Println("Failed to unmarshal", err.Error())
		return
	}
	fmt.Println("Response:", msg)
	entityID, err = strconv.ParseInt(msg.Result, 10, 64)
	if err != nil {
		fmt.Println("Failed to parse response", err.Error(), msg.Result)
		return
	}
	return
}

func testZoneCommand(zone string, command string, params []string) {

	msg := &eqproto.CommandMessage{
		Author:  "xackery",
		Command: command,
		Params:  params,
	}
	d, err := proto.Marshal(msg)
	if err != nil {
		log.Fatal(err)
	}
	reply, err := nc.Request(fmt.Sprintf("zone.%s.command_message", zone), d, 1*time.Second)
	if err != nil {
		log.Println("Failed to get request response:", err.Error())
		return
	}

	err = proto.Unmarshal(reply.Data, msg)
	if err != nil {
		fmt.Println("Failed to unmarshal", err.Error())
		return
	}
	fmt.Println("Response:", msg)
	return
}
func testAsyncEntityEventSubscriber(zone string, entityID int64) {

	/*event := &eqproto.EntityEvent{
		Entity: &eqproto.Entity{
			Id: 1,
		},
	}
	d, err := proto.Marshal(event)
	if err != nil {
		log.Fatal(err)
	}
	if err = nc.Publish(fmt.Sprintf("zone.%s.entity.event_subscribe.all", zone), d); err != nil {
		log.Println("Failed to publish event subscribe:", err.Error())
		return
	}*/

	var opCode int64
	var index int
	channel := fmt.Sprintf("zone.%s.entity.event.%d", zone, entityID)
	nc.Subscribe(channel, func(m *nats.Msg) {
		event := &eqproto.Event{}
		err = proto.Unmarshal(m.Data, event)
		if err != nil {
			fmt.Println("invalid event data passed", m.Data)
		}

		var eventPayload proto.Message
		switch event.Op {
		case eqproto.OpCode_OP_ClientUpdate:
			eventPayload = &eqproto.PlayerPositionUpdateEvent{}
		case eqproto.OpCode_OP_Animation:
			eventPayload = &eqproto.AnimationEvent{}
		case eqproto.OpCode_OP_NewSpawn:
			eventPayload = &eqproto.SpawnEvent{}
		case eqproto.OpCode_OP_ZoneEntry:
			eventPayload = &eqproto.SpawnEvent{}
		case eqproto.OpCode_OP_HPUpdate:
			eventPayload = &eqproto.HPEvent{}
		case eqproto.OpCode_OP_MobHealth:
			eventPayload = &eqproto.HPEvent{}
		case eqproto.OpCode_OP_DeleteSpawn:
			eventPayload = &eqproto.DeleteSpawnEvent{}
		case eqproto.OpCode_OP_Damage:
			eventPayload = &eqproto.DamageEvent{}
		default:
			return
		}
		err = proto.Unmarshal(event.Payload, eventPayload)
		if err != nil {
			fmt.Println("Invalid data passed for opcode", eqproto.OpCode(opCode), err.Error(), string(m.Data[index+1:]))
			return
		}
		fmt.Println(m.Subject, event.Op, eventPayload)
		//log.Printf("Received a message on %s: %s\n", m.Subject, string(m.Data))

		//proto.Unmarshal(m.Data, event)
		//log.Println(event.Op.String(), event.Entity, event.Target)
	})
	log.Println("Subscribed to", channel, ", waiting on messages...")

	time.Sleep(500 * time.Second)
}

func testAsyncChannelMessageSubscriber() {
	nc.Subscribe("ChannelMessageWorld", func(m *nats.Msg) {
		//log.Printf("Received a message: %s\n", string(m.Data))
		message := &eqproto.ChannelMessage{}
		proto.Unmarshal(m.Data, message)
		log.Println(message)
	})
	log.Println("Waiting on messages...")

	time.Sleep(500 * time.Second)
}

func testSyncSubscriber() {
	sub, err := nc.SubscribeSync("ChannelMessage")
	if err != nil {
		log.Fatal(err)
	}

	if m, err := sub.NextMsg(10 * time.Second); err != nil {
		log.Fatal(err)
	} else {
		log.Println(m)
	}
}

func testChannelSubscriber() {
	ch := make(chan *nats.Msg, 64)
	sub, err := nc.ChanSubscribe("ChannelMessage", ch)
	if err != nil {
		log.Println("Error subscribing", err.Error())
		return
	}
	msg := <-ch
	log.Println(sub, msg)
}

/*
func testRequestReply() {
	msg, err := nc.Request("help", []byte("help me"), 10*time.Millisecond)
	nc.Subscribe("help", func(m *Msg) {
		nc.Publish(m.Reply, []byte("I can help!"))
	})
}*/

func testBroadcastMessage() {
	message := &eqproto.ChannelMessage{
		From:    "Someone",
		Message: "Test",
		ChanNum: 5, //5 is ooc, 6 is bc
	}
	d, err := proto.Marshal(message)
	if err != nil {
		log.Fatal(err)
	}
	if err = nc.Publish("ChannelMessageWorld", d); err != nil {
		log.Println("Failed to publish:", err.Error())
		return
	}
	log.Println("Sending message", message)
}

func testZoneMessage(zone string, message string) {

	if err = nc.Publish(zone, []byte(message)); err != nil {
		log.Println("Failed to publish:", err.Error())
		return
	}
	log.Println("Sent message", message, "to zone", zone)
}
