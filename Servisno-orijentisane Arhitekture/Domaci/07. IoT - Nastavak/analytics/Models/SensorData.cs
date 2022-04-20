using Newtonsoft.Json;

namespace analytics.Models
{
    public class SensorData
    {
        [JsonProperty("sensorId")]
        public int SensorId { get; set; }

        [JsonProperty("temperature")]
        public int Temperature { get; set; }

        [JsonProperty("offset")]
        public int Offset { get; set; }

    }
}