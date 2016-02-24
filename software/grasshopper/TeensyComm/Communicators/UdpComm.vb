Imports System.Windows.Threading
Imports System.Threading
Imports System.Net
Imports System.Net.Sockets
Imports TeensyComm.Parsing

Namespace Communicators

    ''' <summary>
    ''' Performs reading of TMessages over UDP
    ''' </summary>
    Public Class UdpComm

        Private _udpc As UdpClient = Nothing
        Private _Ip As IPAddress = Nothing
        Private _Port As Integer = 0
        Private _Remote As IPEndPoint = Nothing

        Private _receivedMessages As New List(Of TMessage)
        Private _interval As Integer = 0

        Private T As Threading.Thread = Nothing
        Private ThisDisp As Dispatcher = Dispatcher.CurrentDispatcher

        Private Event GotMessage(sender As UdpComm, e As TeensyEventArgs)

        ''' <summary>
        ''' TeensyEventArgs holds all the messages which arrived since last timer tick
        ''' </summary>
        ''' <param name="sender"></param>
        ''' <param name="e"></param>
        Public Event MessageReceived(sender As UdpComm, e As TeensyEventArgs)

        ''' <summary>
        ''' That's intended to indicate a device which is sending SOME messages, but they're not recognized as Teensy Messaged by the parser
        ''' </summary> 
        ''' <param name="sender"></param>
        ''' <param name="e"></param>
        ''' <remarks></remarks>
        Public Event IncorrectMessageReceived(sender As UdpComm, e As TeensyEventArgs)
        Public Event TimeoutReached(sender As UdpComm, e As TeensyEventArgs)

        WithEvents St As New System.Timers.Timer(1000)
        Private MessOut As New List(Of TMessage)

        Protected ThreadStop As New ManualResetEvent(False)

        Sub New(Ip As IPAddress, Port As Integer, Interval As Integer)
            _Ip = Ip
            _Port = Port
            _interval = Interval
            St.AutoReset = True
        End Sub

        ''' <summary>
        ''' Start communication
        ''' </summary>
        Sub Start()
            _Remote = New IPEndPoint(IPAddress.Any, _Port)
            _udpc = New UdpClient(_Remote)
            T = New Threading.Thread(AddressOf ReceiveUdpData)
            T.IsBackground = True
            T.Start()
            St.Start()
        End Sub

        ''' <summary>
        ''' Close communication and dispose stuff
        ''' </summary>
        Public Sub Close()
            SyncLock _udpc
                ThreadStop.Set()
                _udpc.Close()
                St.Dispose()
            End SyncLock
        End Sub

        Public ReadOnly Property Name As String
            Get
                Return _Ip.ToString & ":" & _Port
            End Get
        End Property

        Public Property Interval As Integer
            Get
                Return _interval
            End Get
            Set(value As Integer)
                _interval = value
                St.Interval = value
            End Set
        End Property

        Private Sub ReceiveUdpData()
            Try
                Do

                    If Not Check() Then Exit Sub
                    If _udpc Is Nothing Then Exit Do
                    Dim doread As Boolean = True

                    While doread

                        If Not Check() Then Exit Sub

                        If _udpc Is Nothing Then Exit Do
                        Dim buf() As Byte = _udpc.Receive(_Remote)
                        If buf.Length < 2 Then Continue While

                        Dim mess As List(Of TMessage) = ListFromBytes(buf, 2)
                        If mess.Count > 0 Then
                            For i As Integer = 0 To mess.Count - 1 Step 1
                                If mess(i).IsValid Then
                                    _receivedMessages.Add(mess(i))
                                Else
                                    If ThisDisp IsNot Nothing Then
                                        ThisDisp.Invoke(AddressOf OnIncorrectReceived)
                                    End If
                                    _receivedMessages.Clear()
                                    Threading.Thread.Sleep(_interval)
                                    If Not Check() Then Exit Sub
                                End If
                            Next
                        End If
                    End While
                Loop
                If Not Check() Then Exit Sub
            Catch ex As Exception

            Finally

            End Try

        End Sub

        Private Function Check() As Boolean
            If ThreadStop.WaitOne(0) Then
                If _udpc IsNot Nothing Then
                    _udpc.Close()
                    Return False
                End If
            End If
            Return True
        End Function

        Private Sub GetMessagesOut() Handles St.Elapsed
            SyncLock _receivedMessages
                MessOut.AddRange(_receivedMessages)
                If ThisDisp IsNot Nothing Then ThisDisp.Invoke(AddressOf OnMessageReceived)
                _receivedMessages.Clear()
            End SyncLock
        End Sub

        Private Sub OnMessageReceived()
            RaiseEvent MessageReceived(Me, New TeensyEventArgs(_Ip.ToString, MessOut))
            MessOut.Clear()
        End Sub

        Private Sub OnIncorrectReceived()
            RaiseEvent IncorrectMessageReceived(Me, New TeensyEventArgs(_Ip.ToString, _receivedMessages))
        End Sub

        Private Sub OnTimeout()
            RaiseEvent TimeoutReached(Me, New TeensyEventArgs(_Ip.ToString, _receivedMessages))
        End Sub

    End Class

End Namespace
