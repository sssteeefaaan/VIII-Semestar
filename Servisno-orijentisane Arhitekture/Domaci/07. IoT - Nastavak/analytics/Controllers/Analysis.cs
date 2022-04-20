using analytics.Models;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace analytics.Controllers
{
    [ApiController]
    [Route("/api/[controller]")]

    public class AnalysisController : ControllerBase
    {
        private static readonly int TMax = 35;

        public AnalysisController()
        {

        }

        [HttpGet]
        public ActionResult<string> Get() {
            return Ok("Analysis works!");
        }

        [HttpPost]
        public async Task<IActionResult> Post(SensorData data)
        {
            int offset = data.Temperature > TMax ? TMax - data.Temperature : 0;
            if(offset != 0){
                using(var httpClient = new HttpClient())
                {
                    data.Offset = offset;
                    var c = JsonConvert.SerializeObject(data);
                    StringContent content = new StringContent(c, Encoding.UTF8, "application/json");
                    using(var response = await httpClient.PutAsync("http://gateway:3000/set", content))
                    {
                        string apiResponse = await response.Content.ReadAsStringAsync();
                        return new JsonResult(
                            new
                            {
                                resp = apiResponse,
                                message = $"Temperature corrected - {offset}!",
                            }
                        );
                    }
                }
            }
            return new JsonResult(
                new
                {
                    message = "Temperature OK!"
                }
            );
        }
    }
}