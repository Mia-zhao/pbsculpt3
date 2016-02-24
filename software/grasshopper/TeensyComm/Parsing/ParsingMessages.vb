Namespace Parsing

    ''' <summary>
    ''' Q = query, R = response
    ''' </summary>
    ''' <remarks></remarks>
    Public Enum MessageType As SByte
        Q_QueryNumDevices = 1
        R_QueryNumDevices = -1
        Q_QueryIDs = 2
        R_QueryIDs = -2
        Q_Fade = 3
        R_Fade = -3
        Q_Read = 4
        R_Read = -4
        Q_CBLA_Status = 5
        R_CBLA_Status = -5
    End Enum

    Public Module Reading

        ''' <summary>
        ''' Creates a list of messages from bytes
        ''' </summary>
        ''' <param name="Bytes">Buffer bytes</param>
        ''' <param name="Skip">Set the number of bytes to skip betweem the messages</param>
        ''' <returns></returns>
        Public Function ListFromBytes(Bytes() As Byte, Skip As Integer) As List(Of TMessage)
            Dim len As Int64 = Bytes.Length
            Dim out As New List(Of TMessage)
            Dim s As Int64 = 0

            While len > 0
                Dim tm As New TMessage
                Dim this As Int64 = FromBytes(Bytes, tm, s)
                If this = -1 Then
                    Exit While
                End If
                s += this + Skip
                len -= this + Skip
                out.Add(tm)
            End While

            Return out
        End Function


        ''' <summary>
        ''' Parses Bytes to a TMessage, returns the length of the message
        ''' </summary>
        ''' <param name="Bytes"></param>
        ''' <param name="Tm"></param>
        ''' <param name="At"></param>
        ''' <returns></returns>
        Public Function FromBytes(Bytes() As Byte, ByRef Tm As TMessage, Optional At As Int64 = 0) As Int64
            Tm.Time = DateTime.UtcNow
            If Not ReadHeader(Bytes, At, Tm.OriginID, Tm.DestinationID, Tm.Size, Tm.MessageType) Then Return -1
            Dim MessageLng As Int64 = Tm.Size + 11 'data + header
            If MessageLng + At > Bytes.Length Then Return -1

            Select Case Tm.MessageType

                Case MessageType.Q_QueryNumDevices
                'no data here 

                Case MessageType.R_QueryNumDevices
                    Tm.Data.Add(Bytes(At + 11))

                Case MessageType.Q_QueryIDs
                'no data here 

                Case MessageType.R_QueryIDs
                    For i As Integer = 11 To Tm.Size - 1 Step 3
                        Tm.Data.AddRange(BytesToDeviceId(Bytes, At + i))
                    Next

                Case MessageType.Q_Fade
                    Tm.Data.AddRange(BytesToDeviceId(Bytes, At + 11))
                    Tm.Data.Add(Bytes(At + 14))
                    Tm.Data.Add(BytesToInt16(Bytes, At + 15))

                Case MessageType.R_Fade
                    Tm.Data.AddRange(BytesToDeviceId(Bytes, At + 11))
                    Tm.Data.Add(Bytes(At + 14))
                    Tm.Data.Add(BytesToInt16(Bytes, At + 15))

                Case MessageType.Q_Read
                    Tm.Data.AddRange(BytesToDeviceId(Bytes, At + 11))
                    Tm.Data.Add(Bytes(At + 14))

                Case MessageType.R_Read
                    Tm.Data.AddRange(BytesToDeviceId(Bytes, At + 11))
                    Tm.Data.Add(Bytes(At + 14))
                    Tm.Data.Add(BytesToInt16(Bytes, At + 15))

                Case MessageType.R_CBLA_Status
                    Tm.Data.Add(Bytes(At + 11))
                    Tm.Data.Add(Bytes(At + 12))
                    Tm.Data.Add((BytesToInt16(Bytes, At + 13)))

            End Select

            Return MessageLng
        End Function

        Public Function ReadHeader(Bytes() As Byte, At As Integer, ByRef OriginId As Integer, ByRef DestinationId As Integer, ByRef Size As Integer, ByRef MsgType As MessageType) As Boolean
            If At + 11 > Bytes.Length Then Return False

            Try
                Dim destid(3) As Byte
                Dim sourid(3) As Byte
                Dim bsiz(3) As Byte
                Array.Copy(Bytes, At + 0, destid, 1, 3)
                Array.Copy(Bytes, At + 3, sourid, 1, 3)
                Array.Copy(Bytes, At + 7, bsiz, 0, 4)

                MsgType = ByteToSByte(Bytes(6))

                Size = BitConverter.ToInt32(bsiz.Reverse.ToArray, 0)
                DestinationId = BitConverter.ToInt32(destid.Reverse.ToArray, 0)
                OriginId = BitConverter.ToInt32(sourid.Reverse.ToArray, 0)
            Catch ex As Exception
                Return False
            End Try

            Return True
        End Function

        Public Function BytesToDeviceId(B() As Byte, At As Integer) As List(Of Integer)

            Dim devb(2) As Byte
            Array.Copy(B, At, devb, 0, 3)

            Dim l As New List(Of Integer)
            l.Add(devb(0))
            l.Add(devb(1))
            l.Add(devb(2))
            Return l
        End Function

    End Module

    Public Module Writing

        ''' <summary>
        ''' Creates an array of bytes from a TMessage
        ''' </summary>
        ''' <param name="Tm"></param>
        ''' <returns></returns>
        Public Function MsgToBytes(Tm As TMessage) As Byte()
            Dim b As New List(Of Byte)
            b.AddRange(IntegerTo3Bytes(Tm.DestinationID))
            b.AddRange(IntegerTo3Bytes(Tm.OriginID))
            b.Add(SByteToByte(Tm.MessageType))

            Select Case Tm.MessageType
                Case MessageType.Q_QueryNumDevices, MessageType.Q_QueryIDs
                    Dim unt As UInt32 = 0
                    b.AddRange(BitConverter.GetBytes(unt))

                Case MessageType.Q_Fade
                    b.Add(BitConverter.GetBytes(Tm.Data(0))(0))
                    b.Add(BitConverter.GetBytes(Tm.Data(1))(0))
                    b.Add(BitConverter.GetBytes(Tm.Data(2))(0))
                    b.Add(BitConverter.GetBytes(Tm.Data(3))(0))

                    Dim b16(1) As Byte
                    Array.Copy(BitConverter.GetBytes(Tm.Data(4)).Reverse.ToArray, 2, b16, 0, 2)

                    b.AddRange(b16)

                Case MessageType.Q_Read


            End Select


            Return b.ToArray
        End Function

    End Module

    Public Module Common

        Function IntegerTo3Bytes(Val As Integer) As Byte()
            Dim b() As Byte = BitConverter.GetBytes(Val).Reverse.ToArray
            Dim bout(2) As Byte
            Array.Copy(b, 1, bout, 0, 3)
            Return bout
        End Function

        Public Function BytesToSingle(B() As Byte, At As Integer) As Single
            Dim valb(3) As Byte
            Array.Copy(B, At, valb, 0, 4)
            Return BitConverter.ToSingle(valb.Reverse.ToArray, 0)
        End Function

        Public Function BytesToInt16(B() As Byte, At As Integer) As Int16
            Dim valb(1) As Byte
            Array.Copy(B, At, valb, 0, 2)
            Return BitConverter.ToInt16(valb.Reverse.ToArray, 0)
        End Function

        Public Function ByteToSByte(b As Byte) As SByte
            Dim bint As Integer = b
            If bint > 127 Then
                bint -= 256
            End If
            Dim sb As SByte = bint
            Return sb
        End Function

        Public Function SByteToByte(sb As SByte) As Byte
            Dim sint As Integer = sb
            If sb < 0 Then
                sint = 256 + sint
            End If
            Dim b As Byte = sint
            Return b
        End Function

    End Module

End Namespace
